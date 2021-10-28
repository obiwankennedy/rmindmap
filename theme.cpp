#include <robotsettings/theme.hpp>

#include <QColor>
#include <QFile>
#include <QFileInfo>
#include <QQmlInfo>
#include <QRegularExpression>

namespace
{
const QRegularExpression k_regexp_category(
    "^\\[(\\w+)\\]$", QRegularExpression::OptimizeOnFirstUsageOption);
const QRegularExpression k_regexp_categoryParent(
    "^\\[(\\w+):(\\w+)\\]$", QRegularExpression::OptimizeOnFirstUsageOption);
const QRegularExpression k_regexp_color("^color +(\\w+)=(#[0-9a-fA-F]{8,8})$",
    QRegularExpression::OptimizeOnFirstUsageOption);
const QRegularExpression k_regexp_integer(
    "^int +(\\w+)=(\\d+)$", QRegularExpression::OptimizeOnFirstUsageOption);
const QRegularExpression k_regexp_real("^real +(\\w+)=(\\d+\\.\\d*)$",
    QRegularExpression::OptimizeOnFirstUsageOption);
const QRegularExpression k_regexp_text(
    "^text +(\\w+)=(.*)$", QRegularExpression::OptimizeOnFirstUsageOption);
} // namespace

namespace RS
{
QString Theme::s_themePath;

Style::Style(Theme *parent)
  : QQmlPropertyMap(this, parent)
{
}

QVariant Style::updateValue(const QString &key, const QVariant &input)
{
  QtQml::qmlWarning(this)
      << "Style properties should not be changed from the qml";
  return value(key);
}

Theme::Theme(QObject *parent)
  : QObject(parent)
{
  qRegisterMetaType<Style*>("RS::Style*");
  Q_ASSERT(!s_themePath.isEmpty() && QFileInfo(s_themePath).exists());
  loadTheme(s_themePath);
}

Theme::~Theme()
{
  for(auto const &pair : _styles)
    delete pair.second;
}

Theme *Theme::instance()
{
  static Theme theme;
  return &theme;
}

Style *Theme::style()
{
  return style("Global");
}

Style *Theme::style(const QString &name)
{
  auto it = _styles.find(name);
  return it != end(_styles) ? it->second : nullptr;
}

Style *Theme::addStyle(const QString &name)
{
  auto style = this->style(name);
  if(!style)
  {
    style = new Style(this);
    _styles.insert({name, style});
  }
  else
  {
    qWarning() << "Style" << name << "defined multiple times";
  }
  return style;
}

void Theme::loadTheme(QString const &path)
{
  QFile file(path);
  if(!file.open(QIODevice::ReadOnly))
  {
    qWarning() << "Could not read theme file";
    return;
  }

  Style *style = nullptr;
  Style *parentStyle = nullptr;
  while(!file.atEnd())
  {
    QString line = file.readLine();
    line = line.remove('\n');
    line = line.remove('\r');
    if(line.isEmpty())
      continue;

    auto match = k_regexp_category.match(line);
    if(match.hasMatch())
    {
      auto name = match.captured(1);
      style = addStyle(name);
      if(name == "Global")
      {
        _globalStyle = style;
      }
      else
      {
        if(_globalStyle)
          copyFrom(style, _globalStyle);
        else
          qWarning() << "Style Global should be defined first.";
      }
      continue;
    }

    match = k_regexp_categoryParent.match(line);
    if(match.hasMatch())
    {
      auto name = match.captured(1);
      style = addStyle(name);
      parentStyle = this->style(match.captured(2));
      if(parentStyle)
      {
        copyFrom(style, parentStyle);
      }
      else
      {
        qWarning() << "Invalid parent" << match.captured(2);
        copyFrom(style, _globalStyle);
      }
      continue;
    }

    if(style == nullptr)
      continue;

    match = k_regexp_color.match(line);
    if(match.hasMatch())
    {
      style->insert(match.captured(1), QColor(match.captured(2)));
      continue;
    }

    match = k_regexp_integer.match(line);
    if(match.hasMatch())
    {
      style->insert(match.captured(1), match.captured(2).toInt());
      continue;
    }

    match = k_regexp_real.match(line);
    if(match.hasMatch())
    {
      auto m = match.captured(2);
      style->insert(match.captured(1), match.captured(2).toDouble());
      continue;
    }

    match = k_regexp_text.match(line);
    if(match.hasMatch())
    {
      style->insert(match.captured(1), match.captured(2));
      continue;
    }
  }
}

void Theme::copyFrom(Style *style, const Style *parentStyle)
{
  for(auto const &key : parentStyle->keys())
    style->insert(key, parentStyle->value(key));
}

} // namespace RS

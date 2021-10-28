#ifndef ROBOTSETTINGS_THEME_HPP
#define ROBOTSETTINGS_THEME_HPP

#include <QObject>
#include <QQmlPropertyMap>
#include <map>
#include <robotsettings/api.hpp>

namespace RS
{
class Theme;
class RS_API Style : public QQmlPropertyMap
{
  Q_OBJECT
public:
  explicit Style(Theme *parent);

protected:
  QVariant updateValue(const QString &key, const QVariant &input) override;
};

/**
 * @brief The Theme class
 * Theme singleton object
 *
 * Reads a theme.ini file to create Style objects with style properties in it.
 * The first style defined should be Global.
 * Unless specified otherwise all styles inherit from Global.
 * A Style can inherit from a specific already defined style.
 * Properties can be of the following types: color, int, real, text
 *
 * As the theme is meant to be loading at start-up this implementation
 * doesn't support dynamic property edition.
 *
 * example:
 * [Global]
 * int margin=5
 * text appName=Some name
 *
 * [Spinner]
 * real speed=53.
 *
 * [Button]
 * color border=#FF00FF00
 *
 * [WarningButton:Button]
 * color background=#FFFF0000
 *
 */
class RS_API Theme : public QObject
{
  Q_OBJECT
public:
  static QString s_themePath;

  explicit Theme(QObject *parent = nullptr);
  ~Theme();

  static Theme *instance();

  Q_INVOKABLE RS::Style *style();
  Q_INVOKABLE RS::Style *style(QString const &name);

private:
  Style *addStyle(QString const &name);
  void loadTheme(QString const &path);

  void copyFrom(Style *style, Style const *parentStyle);

private:
  Style *_globalStyle = nullptr;
  std::map<QString, Style *> _styles;
};

} // namespace RS

#endif // ROBOTSETTINGS_THEME_HPP

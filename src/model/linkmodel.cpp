#include "linkmodel.h"

#include "data/mindnode.h"

LinkModel::LinkModel(QObject* parent) : QAbstractItemModel(parent) {}

QModelIndex LinkModel::index(int row, int column, const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    if(row < 0)
        return QModelIndex();

    auto const link= m_data.at(static_cast<std::size_t>(row));

    return createIndex(row, column, link);
}

QModelIndex LinkModel::parent(const QModelIndex& index) const
{
    Q_UNUSED(index)
    return {};
}

int LinkModel::rowCount(const QModelIndex& parent) const
{
    if(parent.isValid())
        return 0;

    return static_cast<int>(m_data.size());
}

int LinkModel::columnCount(const QModelIndex& parent) const
{
    if(!parent.isValid())
        return 0;

    return 1;
}

QVariant LinkModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    auto link= m_data.at(static_cast<std::size_t>(index.row()));
    QVariant result;
    switch(role)
    {
    case Direction:
        result= link->direction();
        break;
    case Position:
        result= link->startPoint();
        break;
    case Last:
        result= link->endPoint();
        break;
    case Width:
        result= link->end()->position().x();
        break;
    case Height:
        result= link->end()->position().y();
        break;
    case LinkRole:
        result= QVariant::fromValue(link);
    }

    return result;
}

bool LinkModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    bool ret= false;
    if(index.isValid() && !index.parent().isValid())
    {
        // auto link= m_data.at(index.row());
        ret= true;
        Q_EMIT dataChanged(index, index, QVector<int>() << role);
    }
    return ret;
}

QHash<int, QByteArray> LinkModel::roleNames() const
{
    static QHash<int, QByteArray> roles
        = {{LinkModel::Direction, "direction"}, {LinkModel::Position, "position"}, {LinkModel::Last, "last"},
           {LinkModel::Height, "heightLink"},   {LinkModel::Width, "widthLink"},   {LinkModel::LinkRole, "link"}};
    return roles;
}

Link* LinkModel::addLink(MindNode* p1, MindNode* p2)
{
    beginInsertRows(QModelIndex(), static_cast<int>(m_data.size()), static_cast<int>(m_data.size()));
    auto link= new Link();
    connect(link, &Link::linkChanged, this, &LinkModel::linkHasChanged);
    link->setStart(p1);
    link->setEnd(p2);
    m_data.push_back(link);
    endInsertRows();
    return link;
}
void LinkModel::linkHasChanged()
{
    auto link= qobject_cast<Link*>(sender());
    QModelIndex parent;
    auto it= std::find(m_data.begin(), m_data.end(), link);
    auto offset= std::distance(m_data.begin(), it);
    auto idx1= index(offset, 0, parent);
    Q_EMIT dataChanged(idx1, idx1, QVector<int>() << Position << Last << Width << Height);
}

Qt::ItemFlags LinkModel::flags(const QModelIndex& index) const
{
    auto flags= QAbstractItemModel::flags(index);
    flags|= Qt::ItemIsEditable;
    return flags;
}

std::vector<Link*>& LinkModel::getDataSet()
{
    return m_data;
}
void LinkModel::openLinkAndChildren(const QString& id, bool status)
{
    std::vector<Link*> connectedLinks;
    std::copy_if(m_data.begin(), m_data.end(), std::back_inserter(connectedLinks),
                 [id](const Link* link) { return link->start()->id() == id; });

    std::for_each(connectedLinks.begin(), connectedLinks.end(), [status](Link* link) { link->setVisible(status); });
}

void LinkModel::clear()
{
    beginResetModel();
    qDeleteAll(m_data);
    m_data.clear();
    endResetModel();
}

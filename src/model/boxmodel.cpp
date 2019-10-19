#include "boxmodel.h"

#include <QColor>
#include <QDebug>
#include <QRectF>

BoxModel::BoxModel(QObject* parent) : QAbstractItemModel(parent) {}

int BoxModel::rowCount(const QModelIndex& parent) const
{
    if(!parent.isValid())
        return static_cast<int>(m_data.size());

    return 0;
}

int BoxModel::columnCount(const QModelIndex& parent) const
{
    if(parent.isValid())
        return 0;

    return 1;
}

QModelIndex BoxModel::parent(const QModelIndex& index) const
{
    return QModelIndex();
}

QVariant BoxModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    QVariant result;
    auto mindNode= static_cast<MindNode*>(index.internalPointer());
    switch(role)
    {
    case Color:
        result= QVariant::fromValue(QColor(Qt::gray));
        break;
    case ContentWidth:
        result= mindNode->contentWidth();
        break;
    case ContentHeight:
        result= mindNode->contentHeight();
        break;
    case Label:
        result= mindNode->text();
        break;
    case Position:
        result= mindNode->position();
        break;
    case Posx:
        result= mindNode->position().x();
        break;
    case Posy:
        result= mindNode->position().y();
        break;
    case Node:
        result= QVariant::fromValue(mindNode);
        break;
    }
    return result;
}

bool BoxModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(data(index, role) != value)
    {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

QModelIndex BoxModel::index(int row, int column, const QModelIndex& parent) const
{
    if(row < 0)
        return QModelIndex();

    MindNode* childItem= nullptr;

    if(!parent.isValid())
        childItem= m_data.at(row);

    if(childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QAbstractItemModel* BoxModel::linkModel() const
{
    return m_linkModel;
}

void BoxModel::setLinkModel(QAbstractItemModel* model)
{
    auto linkModel= dynamic_cast<LinkModel*>(model);
    if(linkModel == m_linkModel)
        return;

    m_linkModel= linkModel;
    emit linkModelChanged();
}

Qt::ItemFlags BoxModel::flags(const QModelIndex& index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}

QHash<int, QByteArray> BoxModel::roleNames() const
{
    static QHash<int, QByteArray> roles= {{BoxModel::Label, "label"},
                                          {BoxModel::ContentWidth, "contentWidth"},
                                          {BoxModel::ContentHeight, "contentHeight"},
                                          {BoxModel::Color, "colorbg"},
                                          {BoxModel::Position, "position"},
                                          {BoxModel::Node, "node"},
                                          {BoxModel::Posx, "posx"},
                                          {BoxModel::Posy, "posy"}};
    return roles;
}

void BoxModel::clear()
{
    beginResetModel();
    qDeleteAll(m_data);
    m_data.clear();
    endResetModel();
}

void BoxModel::appendNode(MindNode* node)
{
    auto row= static_cast<int>(m_data.size());
    beginInsertRows(QModelIndex(), row, row);
    m_data.push_back(node);
    endInsertRows();
}

bool BoxModel::addBox(const QString& idparent)
{

    auto row= static_cast<int>(m_data.size());
    beginInsertRows(QModelIndex(), row, row);
    auto root= new MindNode();
    root->setText(tr("New Node"));

    m_data.push_back(root);
    if(idparent.isEmpty())
        return true;

    auto id= std::find_if(m_data.begin(), m_data.end(),
                          [idparent](const MindNode* node) { return idparent == node->id(); });
    if(id == m_data.end())
        return false;

    auto rectParent= (*id)->boundingRect();
    auto pos= rectParent.topLeft() + QPointF(rectParent.width() * 1.5, rectParent.height() * 1.5);
    root->setPosition(pos);
    endInsertRows();

    m_linkModel->addLink(*id, root);

    return true;
}

bool BoxModel::removeBox(const QString& id)
{
    //  beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    //  endRemoveRows();
}

void BoxModel::openNode(const QString& id, bool status)
{
    auto it= std::find_if(m_data.begin(), m_data.end(), [id](const MindNode* node) { return id == node->id(); });
    if(it == m_data.end())
        return;
    if((*it)->open() == status)
        return;

    (*it)->setOpen(status);
}
std::vector<MindNode*>& BoxModel::nodes()
{
    return m_data;
}

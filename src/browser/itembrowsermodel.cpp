#include <QDebug>

#include "itembrowsermodel.h"


TreeItem::TreeItem() :
    m_item(NULL)
{
    m_children = new QList<TreeItem*>;
}

int TreeItem::childrenCount()
{
    return m_children->size();
}
GenericMindMapItem* TreeItem::getData()
{
    return m_item;
}
TreeItem* TreeItem::childAt(int a)
{
    if((m_children->size()>a)&&(a>=0))
    {
        return m_children->at(a);
    }
}
void TreeItem::setData(GenericMindMapItem* p)
{
    m_item = p;
}
int TreeItem::indexOf(TreeItem* p)
{
    return m_children->indexOf(p);
}
void TreeItem::addChild(GenericMindMapItem* item)
{
     TreeItem* p = new TreeItem();
     p->setData(item);
     p->setParent(this);
     m_children->append(p);
}
TreeItem* TreeItem::getParent()
{
    return m_parent;
}
void TreeItem::setParent(TreeItem* p)
{
    m_parent=p;
}
void TreeItem::clear()
{
    foreach(TreeItem* child, *m_children)
    {
        child->clear();
        delete child;

    }
    m_children->clear();
}
void TreeItem::removeChildAt(int i)
{
    m_children->removeAt(i);
}
//////////////////////////////////////////////////////////////////////////////////////////////
// Class ItemBrowserModel
//////////////////////////////////////////////////////////////////////////////////////////////
ItemBrowserModel::ItemBrowserModel(QObject *parent) :
    QAbstractItemModel(parent),m_root(NULL)
{
    m_root=new TreeItem;
}
QModelIndex ItemBrowserModel::index ( int row, int column, const QModelIndex & parent  ) const
{
    //qDebug()<< row << column;
        TreeItem *parentItem;

         if (!parent.isValid())
             parentItem = m_root;
         else
             parentItem = static_cast<TreeItem*>(parent.internalPointer());



         TreeItem *childItem = parentItem->childAt(row);

         if (childItem)
             return createIndex(row, column, childItem);
         else
             return QModelIndex();

}
QModelIndex ItemBrowserModel::parent ( const QModelIndex & index ) const
{
    if (!index.isValid())
             return QModelIndex();

         TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
         TreeItem *parentItem = childItem->getParent();

         if (parentItem == m_root)
             return QModelIndex();

         return createIndex(parentItem->getParent()->indexOf(parentItem->getParent()), 0, parentItem);

}
int ItemBrowserModel::rowCount ( const QModelIndex & parent  ) const
{
   // qDebug() << parent.row() << parent.column() << parent.isValid() << m_root->childrenCount();
    if (parent.isValid())
    {
        return static_cast<TreeItem*>(parent.internalPointer())->childrenCount();
    }
    else if(!parent.isValid())
    {
        return m_root->childrenCount();
    }
}
int ItemBrowserModel::columnCount ( const QModelIndex & parent  ) const
{
    return 1;
}
QVariant ItemBrowserModel::data ( const QModelIndex & index, int role  ) const
{
    if (!index.isValid())
             return QVariant();

    //if(!index.parent().isValid())
      //       return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
         if (role == Qt::DisplayRole)
         {
             return item->getData()->getText();
         }
         else if(role == Qt::DecorationRole)
         {
             return item->getData()->getIcon();
         }
         else
         {
            return QVariant();
         }






}
bool ItemBrowserModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    if(Qt::EditRole==role)
    {
        item->getData()->setText(value.toString());
        return true;
    }
    return QAbstractItemModel::setData(index,value,role);
}

bool ItemBrowserModel::setHeaderData ( int section, Qt::Orientation orientation, const QVariant & value, int role )
{
    return true;
}

Qt::ItemFlags ItemBrowserModel::flags ( const QModelIndex & index ) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEditable| Qt::ItemIsEnabled ;
}
void ItemBrowserModel::insertItem(GenericMindMapItem* p)
{
    beginInsertRows(QModelIndex(),m_root->childrenCount(),m_root->childrenCount());
    m_root->addChild(p);
    endInsertRows();
}
void ItemBrowserModel::clear()
{
    beginRemoveRows(QModelIndex(),0,m_root->childrenCount());
    m_root->clear();
    endRemoveRows();
}
void ItemBrowserModel::removeItem(const QModelIndex & index)
{
    beginRemoveRows(QModelIndex(),index.row(),index.row());
    m_root->removeChildAt(index.row());
    endRemoveRows();
}
GenericMindMapItem* ItemBrowserModel::indexToMindMapItem(const QModelIndex & index)
{
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    if(NULL==item)
        return NULL;

    return item->getData();
}

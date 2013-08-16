#include <QPainter>

#include "items/genericmindmapitem.h"
#include "items/packageitem.h"

#include "graphcommonmodel.h"

GraphCommonModel::GraphCommonModel()
{
    m_list = new QList<PackageItem*>();

}

GraphCommonModel::~GraphCommonModel()
{
    delete m_list;
}

int GraphCommonModel::rowCount ( const QModelIndex & parent ) const
{
    return m_list->size();
}

PackageItem* GraphCommonModel::getNodeAt(const QModelIndex & index)
{
    return m_list->at(index.row());
}

QVariant GraphCommonModel::data ( const QModelIndex & index, int role ) const
{
    if(!index.isValid())
        return QVariant();


        if(Qt::DisplayRole==role)
        {
            return m_list->at(index.row())->getName();
        }
        else if(Qt::DecorationRole==role)
        {
            return  m_list->at(index.row())->getIcon();
        }
        return QVariant();
}
void GraphCommonModel::addItems(PackageItem* node)
{
    beginInsertRows(QModelIndex(),m_list->size(),m_list->size());
    m_list->append(node);
    endInsertRows();

}
void GraphCommonModel::readSettings(QSettings& settings)
{
    settings.beginGroup("GeometricModel");
    int size = settings.beginReadArray("shapes");
    PackageItem* tmp=NULL;
     for (int i = 0; i < size; ++i) {
         settings.setArrayIndex(i);
         tmp=new PackageItem();
         tmp->setText(settings.value("title").toString());
       /*  tmp->setBgColor(settings.value("bgcolor").value<QColor>());
         tmp->setColor(settings.value("color").value<QColor>());*/
         m_list->append(tmp);
     }
     settings.endArray();
     settings.endGroup();

     if(m_list->empty())
     {
        addItems(new PackageItem());
     }
}

void GraphCommonModel::writeSettings(QSettings& settings)
{
    settings.beginGroup("GeometricModel");
    settings.beginWriteArray("shapes");
    for(int i =0;i<m_list->size();++i)
    {
        settings.setArrayIndex(i);
        settings.setValue("title", m_list->at(i)->getText());
        /*settings.setValue("bgcolor", m_list->at(i)->bgColor());
        settings.setValue("color",m_list->at(i)->color());*/

    }
    settings.endArray();
    settings.endGroup();

}

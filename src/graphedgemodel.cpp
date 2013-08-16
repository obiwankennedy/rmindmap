#include "graphedgemodel.h"

GraphEdgeModel::GraphEdgeModel()
{
    m_list = new QList<Edge*>();
}
GraphEdgeModel::~GraphEdgeModel()
{
    delete m_list;
}

int GraphEdgeModel::rowCount ( const QModelIndex & parent ) const
{
    return m_list->size();
}

Edge* GraphEdgeModel::getNodeAt(const QModelIndex & index)
{
    return m_list->at(index.row());
}

QVariant GraphEdgeModel::data ( const QModelIndex & index, int role ) const
{
    if(!index.isValid())
        return QVariant();


        if(role==Qt::DisplayRole)
        {
            return m_list->at(index.row())->getText();
        }
        else if(Qt::DecorationRole==role)
        {
            return  m_list->at(index.row())->getIcon();
        }
        return QVariant();
}
void GraphEdgeModel::addItems(Edge* node)
{
    beginInsertRows(QModelIndex(),m_list->size(),m_list->size());
    m_list->append(node);
    endInsertRows();

}
void GraphEdgeModel::readSettings(QSettings& settings)
{
    settings.beginGroup("EdgeModel");
    int size = settings.beginReadArray("edges");
    Edge* tmp=NULL;
     for (int i = 0; i < size; ++i) {
         settings.setArrayIndex(i);
         tmp=new Edge(NULL);
         tmp->setText(settings.value("text").toString());
         tmp->setKind((Edge::EndKind)settings.value("kind").toInt());
         //tmp->setBgColor(settings.value("bgcolor").value<QColor>());
         m_list->append(tmp);
     }
     settings.endArray();
     settings.endGroup();

    //
    if(m_list->size()<3)
    {
        m_list->clear();

        Edge* tmp=new Edge(NULL);
        //tmp->setColor(Qt::blue);
        tmp->setText(tr("Text"));
        addItems(tmp);

        tmp=new Edge(NULL);
        //tmp->setColor(Qt::blue);
        tmp->setText(tr("End1"));
        tmp->setKind(Edge::END1);
        addItems(tmp);

        tmp=new Edge(NULL);
        //tmp->setColor(Qt::blue);
        tmp->setText(tr("End2"));
        tmp->setKind(Edge::END2);
        addItems(tmp);
    }
}

void GraphEdgeModel::writeSettings(QSettings& settings)
{
    settings.beginGroup("EdgeModel");
    settings.beginWriteArray("edges");
    for(int i =0;i<m_list->size();++i)
    {
        settings.setArrayIndex(i);
        settings.setValue("text", m_list->at(i)->getText());
        settings.setValue("kind", m_list->at(i)->getKind());

    }
    settings.endArray();
    settings.endGroup();

}

#include <QPainter>

#include "graphitemmodel.h"
#include "preferences/preferencesmanager.h"



GraphItemModel::GraphItemModel(StringManager* stringManager)
    : m_stringManager(stringManager)
{
    m_list = new QList<Node*>();
    m_preferences = PreferencesManager::getInstance();
}

GraphItemModel::~GraphItemModel()
{
    delete m_list;
}

int GraphItemModel::rowCount ( const QModelIndex & parent ) const
{
    return m_list->size();
}

Node* GraphItemModel::getNodeAt(const QModelIndex & index)
{
    return m_list->at(index.row());
}

QVariant GraphItemModel::data ( const QModelIndex & index, int role ) const
{
    if(!index.isValid())
        return QVariant();


    if((Qt::DisplayRole==role)||(Qt::EditRole==role))
        {
            return m_list->at(index.row())->getText();
        }
        else if(Qt::DecorationRole==role)
        {
            return  m_list->at(index.row())->getIcon();
        }
        return QVariant();
}
void GraphItemModel::addItems(Node* node)
{
    beginInsertRows(QModelIndex(),m_list->size(),m_list->size());
    m_list->append(node);
    endInsertRows();

}
void GraphItemModel::readSettings(QSettings& settings)
{
    settings.beginGroup("NodeModel");
    int size = settings.beginReadArray("nodes");
    Node* tmp=NULL;
     for (int i = 0; i < size; ++i) {
         settings.setArrayIndex(i);
         tmp=new Node(NULL);
         tmp->setStringManager(m_stringManager);
         tmp->setUuid(settings.value("id").toString());
         tmp->setText(settings.value("text").toString());
         tmp->setBgColor(settings.value("bgcolor").value<QColor>());
         tmp->setColor(settings.value("color").value<QColor>());
         m_list->append(tmp);
     }
     settings.endArray();
     settings.endGroup();


    if(m_list->size()==0)
    {
        Node* tmp=new Node(NULL);
        tmp->setColor(Qt::blue);
        tmp->setStringManager(m_stringManager);
        tmp->setText(tr("Male"));
        addItems(tmp);

        tmp=new Node(NULL);
        tmp->setColor(Qt::red);
        tmp->setText(tr("Female"));
        tmp->setStringManager(m_stringManager);
        addItems(tmp);

        tmp=new Node(NULL);
        tmp->setColor(Qt::green);
        tmp->setText(tr("Place"));
        tmp->setStringManager(m_stringManager);
        addItems(tmp);
    }
}

void GraphItemModel::writeSettings(QSettings& settings)
{
    settings.beginGroup("NodeModel");
    settings.beginWriteArray("nodes");
    for(int i =0;i<m_list->size();++i)
    {
        settings.setArrayIndex(i);
        settings.setValue("text", m_list->at(i)->getText());
        settings.setValue("bgcolor", m_list->at(i)->bgColor());
        settings.setValue("color",m_list->at(i)->color());
        settings.setValue("id",m_list->at(i)->getUuid());

    }
    settings.endArray();
    settings.endGroup();

}

bool GraphItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(Qt::EditRole==role)
    {
        m_list->at(index.row())->setText(value.toString());
        return true;
    }
    return QAbstractListModel::setData(index,value,role);
}
Qt::ItemFlags GraphItemModel::flags ( const QModelIndex & index ) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEditable |   Qt::ItemIsEnabled;
}

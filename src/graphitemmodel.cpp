/***************************************************************************
 *	Copyright (C) 2011 by Renaud Guezennec                             *
 *   http://renaudguezennec.homelinux.org/accueil,3.html                   *
 *                                                                         *
 *   rmindmap is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
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
            //return  m_list->at(index.row())->getIcon();
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
         //tmp->getColorTheme()->readSetting(settings);
         m_list->append(tmp);
     }
     settings.endArray();
     settings.endGroup();


    /*if(m_list->size()==0)
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
    }*/
}

void GraphItemModel::writeSettings(QSettings& settings)
{
    settings.beginGroup("NodeModel");
    settings.beginWriteArray("nodes");
    for(int i =0;i<m_list->size();++i)
    {
        settings.setArrayIndex(i);
        settings.setValue("text", m_list->at(i)->getText());
     //   settings.setValue("bgcolor", m_list->at(i)->bgColor());
       // settings.setValue("color",m_list->at(i)->color());
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

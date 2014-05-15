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
#include <QVBoxLayout>
#include <QLabel>




#include "mindtoolbar.h"

#include "items/packageitem.h"

MindToolBar::MindToolBar(StringManager* stringManager,QWidget *parent) :
    QWidget(parent),m_stringManager(stringManager)
{
    m_model=new GraphItemModel(m_stringManager);
    m_group = new QButtonGroup();
    m_edgeModel = new GraphEdgeModel();
    m_commonModel = new GraphCommonModel();



    setupUi();
}


void MindToolBar::setupUi()
{
    m_itemList = new QListView();
    connect(m_itemList, SIGNAL(pressed(QModelIndex)),this,SLOT(clickOn(QModelIndex)));
    m_itemList->setModel(m_model);

    m_edgeList = new QListView();
    connect(m_edgeList, SIGNAL(pressed(QModelIndex)),this,SLOT(clickOnEdge(QModelIndex)));
    m_edgeList->setModel(m_edgeModel);

    m_geoList = new QListView();
    connect(m_geoList, SIGNAL(pressed(QModelIndex)),this,SLOT(clickOnCommon(QModelIndex)));
    m_geoList->setModel(m_commonModel);

    QVBoxLayout* layout = new QVBoxLayout(this);



    //QHBoxLayout* buttonlayout = new QHBoxLayout();
    /// @todo add icons to push button
    m_handleItem= new QPushButton(tr("Handle"));
    m_handleItem->setCheckable(true);
    layout->addWidget(m_handleItem);



    m_addBreak= new QPushButton(tr("Add Break"));
    m_addBreak->setCheckable(true);
    m_delBreak= new QPushButton(tr("Delete Break"));
    m_delBreak->setCheckable(true);
    layout->addWidget(m_addBreak);
    layout->addWidget(m_delBreak);



    layout->addWidget(new QLabel(tr("Shape:"),this));
    layout->addWidget(m_geoList);

    layout->addWidget(new QLabel(tr("Edge:"),this));
    layout->addWidget(m_edgeList);

    layout->addWidget(new QLabel(tr("Node:"),this));
    layout->addWidget(m_itemList);

    QHBoxLayout* buttonlayout2 = new QHBoxLayout();
    m_addItem= new QPushButton(tr("Add Item"));
    m_addItem->setCheckable(true);
    m_delItem= new QPushButton(tr("Delete Item"));
    m_delItem->setCheckable(true);
    buttonlayout2->addWidget(m_addItem);
    buttonlayout2->addWidget(m_delItem);

    layout->addLayout(buttonlayout2);



    //m_group->addButton(m_arrow);
    m_group->addButton(m_handleItem);
    m_group->addButton(m_delBreak);
    m_group->addButton(m_addBreak);
    m_group->setExclusive(false);


    connect(m_handleItem,SIGNAL(pressed()),this,SLOT(chooseTool()));
    connect(m_addBreak,SIGNAL(pressed()),this,SLOT(chooseTool()));
    connect(this,SIGNAL(currentToolChanged(MindToolBar::MINDTOOL)),this, SLOT(currentToolHasChanged(MindToolBar::MINDTOOL)));
   // connect(m_arrow,SIGNAL(pressed()),this,SLOT(chooseTool()));



}

void MindToolBar::clickOn(QModelIndex index)
{
    emit currentToolChanged(MindToolBar::ADD_ITEM);
    emit currentNodeBrushChanged(m_model->getNodeAt(index));
}
void MindToolBar::clickOnCommon(QModelIndex index)
{
    emit currentToolChanged(MindToolBar::ADD_GEO);
    emit currentGeoTypeChanged(m_commonModel->getNodeAt(index)->getName());
}

void MindToolBar::readSettings(QSettings& settings)
{
    m_model->readSettings(settings);
    m_edgeModel->readSettings(settings);
    m_commonModel->readSettings(settings);
}

void MindToolBar::writeSettings(QSettings& settings)
{
    m_model->writeSettings(settings);
    m_edgeModel->writeSettings(settings);
    m_commonModel->writeSettings(settings);

}
void MindToolBar::clickOnEdge(QModelIndex index)
{
    emit currentToolChanged(MindToolBar::ARROW);
    emit currentEdgeBrushChanged(m_edgeModel->getNodeAt(index));
}

void MindToolBar::chooseTool()
{
    MINDTOOL newTool;
    QObject* emiter = sender();
    if(emiter==m_arrow)
    {
        newTool=MindToolBar::ARROW;
    }
    else if(emiter==m_handleItem)
    {
        newTool=MindToolBar::HANDLE;
    }
    else if(emiter==m_addBreak)
    {
        newTool=MindToolBar::ADD_BREAK;
    }
    else if(emiter==m_delBreak)
    {
        newTool=MindToolBar::DEL_BREAK;
    }


    emit currentToolChanged(newTool);
}
void MindToolBar::addNodeBrush(Node* node)
{
    m_model->addItems(node);
}
void MindToolBar::currentToolHasChanged(MindToolBar::MINDTOOL tool)
{
    //NONE, HANDLE,ADD_ITEM,DEL_ITEM, ARROW, ADD_BREAK, DEL_BREAK , ADD_GEO
    /*
     *
     *
     * QPushButton* m_handleItem;

    QPushButton* m_addItem;
    QPushButton* m_delItem;

    QPushButton* m_arrow;
    QPushButton* m_addBreak;
    QPushButton* m_delBreak;

    QButtonGroup* m_group;
*/
    switch(tool)
    {
    case MindToolBar::NONE:
        //m_arrow->setChecked(false);
        m_addBreak->setChecked(false);
        m_delBreak->setChecked(false);
        m_handleItem->setChecked(false);
        m_addItem->setChecked(false);
        m_delItem->setChecked(false);
        m_itemList->selectionModel()->clear();
        m_edgeList->selectionModel()->clear();
        m_geoList->selectionModel()->clear();
        break;
    case MindToolBar::HANDLE:
        //m_arrow->setChecked(false);
        m_addBreak->setChecked(false);
        m_delBreak->setChecked(false);
        //m_handleItem->setChecked(false);
        m_delItem->setChecked(false);
        m_addItem->setChecked(false);
        m_itemList->selectionModel()->clear();
        m_edgeList->selectionModel()->clear();
        m_geoList->selectionModel()->clear();
        break;
    case MindToolBar::ADD_ITEM:
        //m_arrow->setChecked(false);
        m_addBreak->setChecked(false);
        m_delBreak->setChecked(false);
        m_handleItem->setChecked(false);
        m_delItem->setChecked(false);
        //m_addItem->setChecked(false);
        //m_itemList->selectionModel()->clear();
        m_edgeList->selectionModel()->clear();
        m_geoList->selectionModel()->clear();
        break;
    case MindToolBar::DEL_ITEM:
        //m_arrow->setChecked(false);
        m_addBreak->setChecked(false);
        m_delBreak->setChecked(false);
        m_handleItem->setChecked(false);
        m_addItem->setChecked(false);
        //m_delItem->setChecked(false);
        m_itemList->selectionModel()->clear();
        m_edgeList->selectionModel()->clear();
        m_geoList->selectionModel()->clear();
        break;
    case MindToolBar::ARROW:
        //m_arrow->setChecked(false);
        m_addBreak->setChecked(false);
        m_delBreak->setChecked(false);
        m_handleItem->setChecked(false);
        m_addItem->setChecked(false);
        m_delItem->setChecked(false);
        m_itemList->selectionModel()->clear();
        //m_edgeList->selectionModel()->clear();
        m_geoList->selectionModel()->clear();
        break;
    case MindToolBar::ADD_BREAK:
        //m_arrow->setChecked(false);
        //m_addBreak->setChecked(false);
        m_delBreak->setChecked(false);
        m_handleItem->setChecked(false);
        m_addItem->setChecked(false);
        m_delItem->setChecked(false);
        m_itemList->selectionModel()->clear();
        m_edgeList->selectionModel()->clear();
        m_geoList->selectionModel()->clear();
        break;
    case MindToolBar::DEL_BREAK:
        //m_arrow->setChecked(false);
        m_addBreak->setChecked(false);
        //m_delBreak->setChecked(false);
        m_handleItem->setChecked(false);
        m_addItem->setChecked(false);
        m_delItem->setChecked(false);
        m_itemList->selectionModel()->clear();
        m_edgeList->selectionModel()->clear();
        m_geoList->selectionModel()->clear();
        break;
    case MindToolBar::ADD_GEO:
        //m_arrow->setChecked(false);
        m_addBreak->setChecked(false);
        m_delBreak->setChecked(false);
        m_handleItem->setChecked(false);
        m_addItem->setChecked(false);
        m_delItem->setChecked(false);
        m_itemList->selectionModel()->clear();
        m_edgeList->selectionModel()->clear();
        //m_geoList->selectionModel()->clear();
        break;


    }
}

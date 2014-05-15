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
#include <QMenu>

#include "itembrowser.h"



ItemBrowser::ItemBrowser()
{
    m_model = new ItemBrowserModel();
    m_view = new QTreeView(this);
    m_view->setContextMenuPolicy(Qt::CustomContextMenu);
    m_view->setHeaderHidden(true);
    m_layout = new QVBoxLayout();
    m_layout->addWidget(m_view);
    m_view->setModel(m_model);
    setLayout(m_layout);


    m_removeItem = new QAction(tr("Remove Items"),this);
    connect(m_view,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenu(QPoint)));
    connect(m_removeItem,SIGNAL(triggered()),this,SLOT(removeCurrentItem()));

    connect(m_view,SIGNAL(doubleClicked(QModelIndex)),this, SLOT(editIndex(QModelIndex)));
}
void ItemBrowser::addItem(GenericMindMapItem* p)
{
    m_model->insertItem(p);
}

void ItemBrowser::removeItem(GenericMindMapItem*)
{

}

void ItemBrowser::clear()
{
    m_model->clear();
}
void ItemBrowser::contextMenu(const QPoint p)
{
    QMenu custom;

    custom.addAction(m_removeItem);

    custom.exec(mapToGlobal(p));
}
void ItemBrowser::removeCurrentItem()
{
    GenericMindMapItem* item = m_model->indexToMindMapItem(m_view->currentIndex());
    if(NULL!=item)
    {
        emit itemHasBeenDeleted(item);

    }
    m_model->removeItem(m_view->currentIndex());

}
void ItemBrowser::editIndex(const QModelIndex& index)
{
     GenericMindMapItem* item = m_model->indexToMindMapItem(index);
     emit selectionChanged(item);
}

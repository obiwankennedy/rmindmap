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
#ifndef ITEMBROWSER_H
#define ITEMBROWSER_H



#include <QWidget>
#include <QTreeView>

#include <QAction>

#include "itembrowsermodel.h"
class QVBoxLayout;

class ItemBrowser : public QWidget
{
    Q_OBJECT

public:
    ItemBrowser();


public slots:
    void addItem(GenericMindMapItem*);
    void removeItem(GenericMindMapItem*);
    void clear();


    void editIndex(const QModelIndex& index);

signals:
    void selectionChanged(GenericMindMapItem*);
    void itemHasBeenDeleted(GenericMindMapItem* item);

private slots:
    void contextMenu(const QPoint p);
    void removeCurrentItem();
private:
    QTreeView* m_view;
    ItemBrowserModel* m_model;
    QVBoxLayout* m_layout;
    QAction* m_removeItem;

};

#endif // ITEMBROWSER_H

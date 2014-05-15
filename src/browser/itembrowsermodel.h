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
#ifndef ITEMBROWSERMODEL_H
#define ITEMBROWSERMODEL_H

#include <QAbstractItemModel>

#include "items/genericmindmapitem.h"


class TreeItem
{
public:
    TreeItem();
    int childrenCount();

    void addChild(GenericMindMapItem*);
    void removeChildAt(int);
    void setData(GenericMindMapItem*);

    int indexOf(TreeItem*);

    TreeItem* childAt(int a);
    TreeItem* getParent();

    GenericMindMapItem* getData();

    void setParent(TreeItem* p);
    void clear();

private:
    QList<TreeItem*>* m_children;
    GenericMindMapItem* m_item;
    TreeItem* m_parent;
    bool isLeaf;
};

class ItemBrowserModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit ItemBrowserModel(QObject *parent = 0);

    void insertItem(GenericMindMapItem*);

    QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
    QModelIndex parent ( const QModelIndex & index ) const;
    int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;


    bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
    bool setHeaderData ( int section, Qt::Orientation orientation, const QVariant & value, int role = Qt::EditRole );

    Qt::ItemFlags flags ( const QModelIndex & index ) const;
    void clear();

    void removeItem(const QModelIndex & index);
    GenericMindMapItem* indexToMindMapItem(const QModelIndex & index);

public slots:
private:
    TreeItem* m_root;
};

#endif // ITEMBROWSERMODEL_H

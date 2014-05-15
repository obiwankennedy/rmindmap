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
#ifndef GRAPHCOMMONMODEL_H
#define GRAPHCOMMONMODEL_H

#include <QAbstractListModel>

#include "serializable.h"

class PackageItem;

class GraphCommonModel : public QAbstractListModel, public SettingsUser
{
public:
    GraphCommonModel();
    ~GraphCommonModel();
    int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    void addItems(PackageItem* node);



    PackageItem* getNodeAt(const QModelIndex & index);



    void readSettings(QSettings& settings);
    void writeSettings(QSettings& settings);


private:
    QList<PackageItem*>* m_list;
};

#endif // GRAPHCOMMONMODEL_H

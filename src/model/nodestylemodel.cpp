/***************************************************************************
 *	Copyright (C) 2019 by Renaud Guezennec                               *
 *   http://www.rolisteam.org/contact                                      *
 *                                                                         *
 *   This software is free software; you can redistribute it and/or modify *
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
#include "nodestylemodel.h"

#include "data/nodestyle.h"

NodeStyleModel::NodeStyleModel(QObject* parent) : QAbstractListModel(parent) {}

int NodeStyleModel::rowCount(const QModelIndex& parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if(parent.isValid())
        return 0;

    return static_cast<int>(m_styles.size());
}

QVariant NodeStyleModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}

QHash<int, QByteArray> NodeStyleModel::roleNames() const
{
    // clang-format off
    static QHash<int, QByteArray> roles
        =  {{NodeStyleModel::ColorOne, "colorOne"},
            {NodeStyleModel::ColorTwo, "colorTwo"},
            {NodeStyleModel::TextColor, "textColor"}};
    //clang-format on
    return roles;
}

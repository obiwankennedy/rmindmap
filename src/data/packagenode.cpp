/***************************************************************************
 *	Copyright (C) 2022 by Renaud Guezennec                               *
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
#include "packagenode.h"

PackageNode::PackageNode(QObject* parent) : PositionedItem{MindItem::PackageType, parent}
{
    connect(this, &PackageNode::positionChanged, this, [this](const QPointF& motion) {
        std::for_each(std::begin(m_internalChildren), std::end(m_internalChildren), [motion](PositionedItem* item) {
            if(!item)
                return;
            auto pos= item->position();
            item->setPosition({pos.x() + motion.x(), pos.y() + motion.y()});
        });
    });
}

const QString& PackageNode::title() const
{
    return m_title;
}

void PackageNode::setTitle(const QString& newTitle)
{
    if(m_title == newTitle)
        return;
    m_title= newTitle;
    emit titleChanged();
}

void PackageNode::addChild(PositionedItem* item)
{
    if(m_internalChildren.contains(item) || item == this)
        return;
    m_internalChildren.append(item);
}

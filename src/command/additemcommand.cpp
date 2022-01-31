/***************************************************************************
 *	Copyright (C) 2019 by Renaud Guezennec                                 *
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
#include "additemcommand.h"

//#include "model/linkmodel.h"
#include "controller/mindmapcontroller.h"
#include "model/minditemmodel.h"

AddItemCommand::AddItemCommand(MindItemModel* nodeModel, MindItem::Type type, MindMapController* ctrl,
                               const QString& idParent, QPointF pos)
    : m_ctrl(ctrl), m_nodeModel(nodeModel), m_idParent(idParent), m_type(type), m_pos(pos)
{
}

void AddItemCommand::undo()
{
    m_nodeModel->removeItem(m_mindItem);
    m_nodeModel->removeItem(m_link);
}

void AddItemCommand::redo()
{
    if(m_mindItem.isNull())
    {
        auto pair= m_nodeModel->addItem(m_idParent, m_type);
        if(!m_pos.isNull())
        {
            auto p= dynamic_cast<PositionedItem*>(pair.first);
            if(p)
                p->setPosition(m_pos);

            if(m_type == MindItem::PackageType)
                m_ctrl->setCurrentPackage(p);
        }
        m_mindItem= pair.first;
        m_link= pair.second;
    }
    else
    {
        m_nodeModel->appendItem(m_mindItem.data());
        m_nodeModel->appendItem(m_link);
    }
}

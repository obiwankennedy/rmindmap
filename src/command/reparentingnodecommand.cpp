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
#include "reparentingnodecommand.h"

//#include "model/linkmodel.h"
#include "data/positioneditem.h"
#include "model/minditemmodel.h"
#include <QDebug>

ReparentingNodeCommand::ReparentingNodeCommand(MindItemModel* nodeModel, PositionedItem* newParent, const QString& id)
    : m_nodeModel(nodeModel), m_newParent(newParent)
{
    m_mindNode= dynamic_cast<PositionedItem*>(m_nodeModel->item(id));
    qDebug() << "node:" << m_mindNode;
    if(m_mindNode)
        m_oldParent= m_mindNode->parentNode();

    qDebug() << "old parent node:" << m_oldParent;
    if(m_oldParent)
    {
        auto links= m_oldParent->subLinks();
        auto idxLink= std::find_if(links.begin(), links.end(),
                                   [this](LinkController* link) { return link->end() == m_mindNode.data(); });
        if(idxLink != links.end())
            m_oldLink= (*idxLink);
    }
}

void ReparentingNodeCommand::undo()
{
    qDebug() << "undo avant:";
    if(m_mindNode.isNull() || m_oldLink.isNull() || m_newLink.isNull())
        return;
    qDebug() << "undo after:";

    m_nodeModel->removeItem(m_newLink);
    m_nodeModel->appendItem(m_oldLink);
}

void ReparentingNodeCommand::redo()
{
    if(m_mindNode.isNull() || m_oldLink.isNull())
        return;

    m_nodeModel->removeItem(m_oldLink);
    if(m_newLink.isNull())
    {
        m_newLink= new LinkController(); // m_nodeModel->addLink(m_newParent, m_mindNode);
        m_newLink->setStart(m_newParent);
        m_newLink->setEnd(m_mindNode);
    }

    m_nodeModel->appendItem(m_newLink);
}

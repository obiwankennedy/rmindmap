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
#include "selectioncontroller.h"

#include "command/dragnodecommand.h"
#include "data/mindnode.h"

SelectionController::SelectionController(QObject* parent) : QObject(parent) {}

void SelectionController::setEnabled(bool enable)
{
    if(enable == m_enabled)
        return;
    m_enabled= enable;
    emit enabledChanged();
}

void SelectionController::setUndoStack(QUndoStack* stack)
{
    m_undoStack= stack;
}

bool SelectionController::enabled() const
{
    return m_enabled;
}

const std::vector<MindItem*>& SelectionController::selectedNodes() const
{
    return m_selection;
}

void SelectionController::addToSelection(MindItem* node)
{
    if(node == nullptr)
        return;
    m_selection.push_back(node);
    node->setSelected(true);
    if(node->type() == MindItem::NodeType || node->type() == MindItem::PackageType)
    {
        auto mindNode= dynamic_cast<PositionedItem*>(node);
        connect(mindNode, &PositionedItem::itemDragged, this, &SelectionController::movingNode);
    }
    emit hasSelectionChanged();
}

void SelectionController::removeFromSelection(MindItem* node)
{
    node->setSelected(false);
    if(node->type() == MindItem::NodeType || node->type() == MindItem::PackageType)
    {
        auto mindNode= dynamic_cast<PositionedItem*>(node);
        disconnect(mindNode, &PositionedItem::itemDragged, this, &SelectionController::movingNode);
    }
    m_selection.erase(std::find(m_selection.begin(), m_selection.end(), node));
    emit hasSelectionChanged();
}

void SelectionController::clearSelection()
{
    std::for_each(m_selection.begin(), m_selection.end(), [this](MindItem* node) {
        node->setSelected(false);
        if(node->type() == MindItem::NodeType || node->type() == MindItem::PackageType)
        {
            auto mindNode= dynamic_cast<PositionedItem*>(node);
            disconnect(mindNode, &PositionedItem::itemDragged, this, &SelectionController::movingNode);
        }
    });
    m_selection.clear();
    emit hasSelectionChanged();
}

void SelectionController::movingNode(const QPointF& motion)
{
    std::vector<QPointer<PositionedItem>> vec;
    std::transform(m_selection.begin(), m_selection.end(), std::back_inserter(vec),
                   [](MindItem* node) { return QPointer<PositionedItem>(dynamic_cast<PositionedItem*>(node)); });
    auto cmd= new DragNodeCommand(motion, vec);

    m_undoStack->push(cmd);
}

bool SelectionController::hasSelection() const
{
    return !m_selection.empty();
}

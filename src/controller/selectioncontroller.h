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
#ifndef SELECTIONCONTROLLER_H
#define SELECTIONCONTROLLER_H

#include <QObject>
#include <QPointF>
#include <QUndoStack>

class MindItem;
class SelectionController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(bool hasSelection READ hasSelection NOTIFY hasSelectionChanged)
public:
    explicit SelectionController(QObject* parent= nullptr);

    void setUndoStack(QUndoStack* stack);

    void setEnabled(bool enable);
    bool enabled() const;
    const std::vector<MindItem*>& selectedNodes() const;
    bool hasSelection() const;

signals:
    void enabledChanged();

    void hasSelectionChanged();

public slots:
    void addToSelection(MindItem* node);
    void removeFromSelection(MindItem* node);
    void movingNode(const QPointF& motion);
    void clearSelection();

private:
    std::vector<MindItem*> m_selection;
    bool m_enabled= false;
    QUndoStack* m_undoStack;
};

#endif // SELECTIONCONTROLLER_H

/***************************************************************************
    *   Copyright (C) 2016 by Renaud Guezennec                                *
    *   http://www.rolisteam.org/contact                                      *
    *                                                                         *
    *   rolisteam is free software; you can redistribute it and/or modify     *
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

#ifndef SELECTIONITEM_H
#define SELECTIONITEM_H

#include <QGraphicsObject>
#include <QGraphicsSceneMouseEvent>
#include "node.h"

#include "edge.h"
/////////////////////////////////
/// \brief HandleItem::HandleItem
/// \param parent
//////////////////////////////////
class AddChildItem : public QGraphicsObject
{
    Q_OBJECT
public:
    AddChildItem(Node* parent);
    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:
    void addNode();
protected:
   virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
   virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
   virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

private:
    Node* m_parent;
    Node* m_currentNode;
};

/////////////////////////////////
/// \brief HandleItem::HandleItem
/// \param parent
//////////////////////////////////
class SelectStyleItem : public QGraphicsObject
{
public:
    SelectStyleItem(Node* parent);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
private:
    Node* m_parent;
};
/////////////////////////////////
/// \brief HandleItem::HandleItem
/// \param parent
//////////////////////////////////
class FoldItem : public QGraphicsObject
{
    Q_OBJECT
public:
    FoldItem(Node* parent);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    bool isFold() const;
    void setFold(bool fold);

    bool hasChild() const;
    void setHasChild(bool hasChild);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
private:
    Node* m_parent;
    bool m_fold;
    bool m_hasChild;
};
/////////////////////////////////
/// \brief HandleItem::HandleItem
/// \param parent
//////////////////////////////////
class SelectionItem : public QGraphicsObject
{
public:
    SelectionItem(Node* parent);
    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    AddChildItem *child() const;
    void setChild(AddChildItem *child);

    FoldItem *folder() const;
    void setFolder(FoldItem *folder);

    void setVisibleStatus(bool isSelected);

private:
    Node* m_parent;
    AddChildItem* m_child;
    SelectStyleItem* m_menu;
    FoldItem* m_folder;

};

#endif // SELECTIONITEM_H

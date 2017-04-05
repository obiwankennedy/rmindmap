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

#include "selectionitem.h"
#include <QPainter>
#include <QDebug>

#define PEN_WIDTH 4
#define BUTTON_SIZE 15
#define ARROW_SIZE 10

#include <QGraphicsScene>

SelectionItem::SelectionItem(Node *parent)
    : m_parent(parent)
{
    setParentItem(parent);

    m_child = new AddChildItem(parent);
    m_child->setParentItem(this);

    m_menu = new SelectStyleItem(parent);
    m_menu->setParentItem(this);

}

QRectF SelectionItem::boundingRect() const
{
    qreal penW = PEN_WIDTH/2;

    return m_parent->boundingRect();//.adjusted(-penW,-penW,penW,penW);
}

void SelectionItem::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(m_parent->isSelected())
    {
        m_child->setPos(boundingRect().width()/2-ARROW_SIZE/2,boundingRect().height()-ARROW_SIZE);
        m_menu->setPos(boundingRect().topRight().x()-BUTTON_SIZE/2,boundingRect().topRight().y()-BUTTON_SIZE/2);
        painter->save();
        QPen pen;
        pen.setColor(Qt::blue);
        pen.setWidth(PEN_WIDTH);

        painter->setPen(pen);
        painter->drawRoundedRect(boundingRect(),m_parent->getRadius(),m_parent->getRadius());
        painter->restore();
    }
}
/////////////////////////////////
/// \brief HandleItem::HandleItem
/// \param parent
//////////////////////////////////

AddChildItem::AddChildItem(Node *parent)
    : m_parent(parent)
{
    setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemSendsGeometryChanges|QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsFocusable);
}

QRectF AddChildItem::boundingRect() const
{
    QRectF rect;
    rect.setCoords(0,0,ARROW_SIZE,ARROW_SIZE);
    return rect;
}

void AddChildItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(m_parent->isSelected())
    {
        painter->save();
       // painter->drawRect(boundingRect());
        QPen pen;
        pen.setColor(Qt::blue);
        pen.setWidth(PEN_WIDTH/2);

        painter->setPen(pen);
        QPainterPath path;

        QPointF point1(ARROW_SIZE/2,0);
        QPointF point2(ARROW_SIZE,ARROW_SIZE);
        QPointF point3(0,ARROW_SIZE);

        path.moveTo(point1);
        path.lineTo(point2);
        path.lineTo(point3);
        path.lineTo(point1);

        painter->fillPath(path,Qt::blue);

        painter->restore();
    }
}

void AddChildItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() << "mouse Press Addchild";
    m_currentNode = new Node();
    m_currentNode->setColorTheme(m_parent->getColorTheme());
    Edge* edge = new Edge(m_parent,m_currentNode);
    edge->setText(" ");
    m_currentNode->setText(" ");
    scene()->addItem(m_currentNode);
    scene()->addItem(edge);
    m_currentNode->setPos(m_parent->pos());
    m_currentNode->setFocus(Qt::MouseFocusReason);

    //m_parent->addEdge(edge);
    //edge->addEdgableItem(m_currentNode);
   // QGraphicsItem::mousePressEvent(event);
}

void AddChildItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
//    qDebug() << "mouseReleaseEvent";
    m_currentNode = NULL;
    QGraphicsItem::mouseReleaseEvent(event);
}
void AddChildItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
 //   qDebug() << "AddChildItem mouseMoveEvent" << event->scenePos();
    if(nullptr != m_currentNode)
    {
        m_currentNode->setPos(event->scenePos());
    }
    //QGraphicsItem::mouseMoveEvent(event);
}
/////////////////////////////////
/// \brief HandleItem::HandleItem
/// \param parent
//////////////////////////////////
///
SelectStyleItem::SelectStyleItem(Node *parent)
: m_parent(parent)
{
   setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemSendsGeometryChanges|QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsFocusable);
}

QRectF SelectStyleItem::boundingRect() const
{
    QRectF rect;
    rect.setCoords(0,0,BUTTON_SIZE,BUTTON_SIZE);
    return rect;
}

void SelectStyleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(m_parent->isSelected())
    {
        painter->save();
       // painter->drawRect(boundingRect());
        QPen pen;
        pen.setColor(Qt::blue);

        painter->setPen(pen);
        painter->setBrush(Qt::blue);
        painter->drawEllipse(boundingRect());
        painter->restore();
    }
}

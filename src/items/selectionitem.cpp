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

#include <QPainter>
#include <QDebug>
#include <QGraphicsScene>

#define PEN_WIDTH 4
#define BUTTON_SIZE 15
#define ARROW_SIZE 10

#include "selectionitem.h"
#include "menustyleitem.h"

SelectionItem::SelectionItem(Node *parent)
    : m_parent(parent)
{
    setParentItem(parent);

    m_child = new AddChildItem(parent);
    m_child->setParentItem(this);

    m_menu = new SelectStyleItem(parent);
    m_menu->setParentItem(this);


    m_folder = new FoldItem(parent);
    m_folder->setParentItem(this);
    m_folder->setZValue(1);

    m_folder->setPos(boundingRect().width()/2-BUTTON_SIZE/2,boundingRect().height());


    connect(m_child,&AddChildItem::addNode,[=](){
         m_folder->setHasChild(true);
    });

}

QRectF SelectionItem::boundingRect() const
{
    //qreal penW = PEN_WIDTH/2;

    return m_parent->boundingRect();//.adjusted(-penW,-penW,penW,penW);
}

void SelectionItem::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if(m_parent->isSelected())
    {
        qreal x = boundingRect().width()/2.0-((qreal)ARROW_SIZE/2.0);
        qreal y = boundingRect().height()-(ARROW_SIZE);

        m_child->setPos(x,y);
        m_menu->setPos(boundingRect().topRight().x()-BUTTON_SIZE/2,boundingRect().topRight().y()-BUTTON_SIZE/2);
        m_folder->setPos(boundingRect().width()/2-BUTTON_SIZE/2,boundingRect().height());

        painter->save();
        QPen pen;
        pen.setColor(Qt::blue);
        pen.setWidth(PEN_WIDTH);

        painter->setPen(pen);
        painter->drawRoundedRect(boundingRect(),m_parent->getRadius(),m_parent->getRadius());
        painter->restore();
    }
}

AddChildItem *SelectionItem::child() const
{
    return m_child;
}

void SelectionItem::setChild(AddChildItem *child)
{
    m_child = child;
}

FoldItem *SelectionItem::folder() const
{
    return m_folder;
}

void SelectionItem::setFolder(FoldItem *folder)
{
    m_folder = folder;
}

void SelectionItem::setVisibleStatus(bool isSelected)
{
    if(isSelected)
    {
        m_folder->setVisible(isSelected);
    }
    else
    {
        m_folder->setVisible(m_folder->isFold());
    }
    m_child->setVisible(isSelected);
    m_menu->setVisible(false);
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
   // qDebug() << "mouse Press Addchild";
    m_currentNode = new Node();
    m_currentNode->setParentItem(m_parent);
    m_currentNode->setColorTheme(m_parent->getColorTheme());
    Edge* edge = new Edge(m_parent,m_currentNode);
    edge->setText(" ");
    edge->setParentItem(m_parent);
    m_currentNode->setText(" ");
    scene()->addItem(m_currentNode);
    scene()->addItem(edge);
    m_currentNode->setPos(event->pos());
    m_currentNode->setFocus(Qt::MouseFocusReason);
   // QGraphicsItem::mousePressEvent(event);
}

void AddChildItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
   // qDebug() << "mouseReleaseEvent";
    m_currentNode = NULL;
    QGraphicsItem::mouseReleaseEvent(event);
}
void AddChildItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
//    qDebug() << "AddChildItem mouseMoveEvent" << event->scenePos();
    if(nullptr != m_currentNode)
    {
        m_currentNode->setPos(event->pos());
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
void SelectStyleItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  //  menu;
 //   menu
   // qDebug() << boundingRect().contains(event->pos()) << "#################";
    if(boundingRect().contains(event->pos()))
    {
        MenuStyleItem menu;
        menu.setItem(m_parent);
        menu.move(event->screenPos());

       menu.exec();
    }
}

FoldItem::FoldItem(Node *parent)
: m_parent(parent),m_fold(false),m_hasChild(false)
{
    setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemSendsGeometryChanges|QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsFocusable);
}

QRectF FoldItem::boundingRect() const
{
    QRectF rect;
    rect.setCoords(0,0,BUTTON_SIZE,BUTTON_SIZE);
    return rect;
}

void FoldItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
        painter->save();
        QPen pen;
        pen.setColor(Qt::black);
        pen.setWidth(1);

        painter->setPen(pen);
        //painter->setBrush(Qt::red);
        painter->drawEllipse(boundingRect());
        painter->setPen(Qt::black);

        QFont font = painter->font();
        font.setBold(true);
        font.setPixelSize(15);
        painter->setFont(font);

        painter->drawText(boundingRect(),Qt::AlignCenter,m_fold ? "+":"-");
        painter->drawRect(boundingRect());
        painter->restore();
}

void FoldItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_fold = !m_fold;

        for(auto item : m_parent->childItems())
        {
            GenericMindMapItem* eItem = dynamic_cast<GenericMindMapItem*>(item);
           // qDebug() << "item" << item << "eItem" << eItem;
            if(nullptr != eItem)
            {
                eItem->setVisible(!m_fold);
            }
        }
        event->accept();
    }
}

bool FoldItem::hasChild() const
{
    return m_hasChild;
}

void FoldItem::setHasChild(bool hasChild)
{
    m_hasChild = hasChild;
}

bool FoldItem::isFold() const
{
    return m_fold;
}

void FoldItem::setFold(bool fold)
{
    m_fold = fold;
}

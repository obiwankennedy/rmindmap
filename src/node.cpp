/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QStyleOption>
#include <QUuid>
#include <QGraphicsProxyWidget>





#include "edge.h"
#include "node.h"
#include "graphwidget.h"



Node::Node(GraphWidget *graphWidget)
    : graph(graphWidget),m_bgcolor(255,255,255),m_color(0,0,0)
{
    m_proxyw = NULL;
    m_text="test";
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    //setCacheMode(DeviceCoordinateCache);
    setCacheMode(NoCache);
    setZValue(-1);
    m_id = QUuid::createUuid().toString();


    m_tempedit = new QLineEdit();
    m_tempedit->setFrame(false);
    m_tempedit->setEnabled(true);
    m_tempedit->setFocus();
    connect(m_tempedit,SIGNAL(textChanged(QString)),this,SLOT(setText(QString)));
    connect(m_tempedit,SIGNAL(editingFinished()),m_tempedit,SLOT(hide()));
    connect(m_tempedit,SIGNAL(textChanged(QString)),this,SLOT(updatePainting()));

}
QString Node::getUuid()
{
    return m_id;
}

void Node::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

QList<Edge *> Node::edges() const
{
    return edgeList;
}

QRectF Node::boundingRect() const
{
    QRectF tmp=m_textRect;


    tmp.setWidth(tmp.width()*2);
    tmp.moveRight(tmp.width()/2);
    tmp.setHeight(tmp.height()*2);

    //qDebug() << "Node rect:" << tmp << " pos:"<< pos() << " parent:"<< parent();

    return tmp;
}

QPainterPath Node::shape() const
{
    QPainterPath path;


    QRectF tmp=m_textRect;
    tmp.setWidth(tmp.width()*2);
    tmp.moveRight(tmp.width()/2);
    tmp.setHeight(tmp.height()*2);

    path.addEllipse(tmp);

    return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    QRect tmp=m_textRect;


    tmp.setWidth(tmp.width()*2);
    tmp.moveRight(tmp.width()/2);
    tmp.setHeight(tmp.height()*2);




    if(QStyle::State_Selected & option->state)
    {
        painter->setPen(QPen(m_color));
        QBrush brush(Qt::black);

        painter->save();
        painter->setBrush(brush);


        painter->drawEllipse(tmp);
        painter->restore();

        painter->setPen(QPen(Qt::white));
        painter->drawText(tmp,Qt::AlignCenter,m_text);
    }
    else
    {
        painter->setPen(QPen(m_color));


        painter->drawEllipse(tmp);
       // painter->drawEllipse(-10,-10,20,20);


        painter->setPen(QPen(Qt::black));
        painter->drawText(tmp,Qt::AlignCenter,m_text);
    }
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemScenePositionHasChanged:
    case ItemPositionHasChanged:
         updateEdges();
         break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}
void Node::updateEdges()
{
    foreach (Edge *edge, edgeList)
    {
        edge->adjust();
    }
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event )
{
    Q_UNUSED(event);
    if(NULL==m_proxyw)
    {
        m_proxyw = scene()->addWidget(m_tempedit);
    }
    m_proxyw->setPos(pos().x()-boundingRect().width(),pos().y()-boundingRect().height());
    m_tempedit->setText(m_text);
    m_tempedit->selectAll();
    m_tempedit->setFocus(Qt::OtherFocusReason);
    m_tempedit->show();

}

void Node::setText(QString text)
{
    m_text=text;
    QFont font;
    QFontMetrics info(font);
    m_textRect = info.boundingRect(m_text);
    if(NULL!=m_stringManager)
    {
        m_stringManager->setValue(tr("%1_%2").arg(m_id).arg("text"),text);
    }
}

QString Node::getText() const
{
    if(NULL!=m_stringManager)
    {
        return m_stringManager->getValue(tr("%1_%2").arg(m_id).arg("text"));
    }
    return QString();
}


void Node::setColor(QColor color)
{
    m_color = color;
    update();
}

QColor Node::color() const
{
    return m_color;
}

void Node::setBgColor(QColor color)
{
   m_bgcolor = color;
   update();
}

QColor Node::bgColor() const
{
    return m_bgcolor;
}
void  Node::updatePainting()
{
    update();
}
QString Node::description() const
{
    return m_description;
}

void Node::setDescription(QString desc)
{
    m_description = desc;
}

void Node::readFromData(QDataStream& in)
{
    in >> m_text;
    setText(m_text);
    in >> m_bgcolor;
    in >> m_color;
    in >> m_id;
    QPointF point;
    in >> point;

    setPos(point);
    in >> m_description;

}

void Node::writeToData(QDataStream& out)
{
    out << m_text;
    out << m_bgcolor;
    out << m_color;
    out << m_id;
    out << pos();
    out << m_description;
}
void Node::setName(QString&)
{

}

QString Node::getName() const
{
    return tr("Node");
}
QPixmap Node::getIcon() const
{
    QPixmap pixmap(30,20);
    QPainter painter(&pixmap);
    painter.fillRect(pixmap.rect(),Qt::white);
    painter.setPen(color());
    painter.drawEllipse(pixmap.rect().adjusted(0,0,-1,-1));
    painter.end();
    return pixmap;
}
QPointF Node::middlePoint()
{
    return QPointF(0,0);
}

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

#include <QPainter>
#include <QGraphicsProxyWidget>

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <math.h>


#include "edge.h"
#include "items/genericmindmapitem.h"
#include "graphwidget.h"



static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;
static const int Arrow_Size = 8;

Edge::Edge()
 : m_arrowSize(Arrow_Size)
{
    init();
}

Edge::Edge(EdgableItems *sourceNode,EdgableItems *destNode)
    : m_arrowSize(Arrow_Size)
{
    init();
    m_source = sourceNode;
    m_dest = destNode;
    m_source->addEdge(this);
    m_dest->addEdge(this);
    adjust();

}
Edge::Edge(EdgableItems *sourceNode)
    : m_arrowSize(Arrow_Size)
{
    init();


    m_dest = NULL;
    qDebug()<< sourceNode;
    if(sourceNode!=NULL)
    {
        m_source = sourceNode;
        m_source->addEdge(this);
        adjust();
    }
}
void Edge::init()
{
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setCacheMode(NoCache);


     m_endkind = BOTH;
     m_proxyw =NULL;
     m_tempedit = new QLineEdit();
     m_tempedit->setFrame(false);
     m_tempedit->setEnabled(true);
     m_tempedit->setFocus();
     connect(m_tempedit,SIGNAL(textChanged(QString)),this,SLOT(setText(QString)));
     connect(m_tempedit,SIGNAL(editingFinished()),m_tempedit,SLOT(hide()));
     connect(m_tempedit,SIGNAL(textChanged(QString)),this,SLOT(updatePainting()));
}

EdgableItems *Edge::sourceNode() const
{
    return m_source;
}

EdgableItems *Edge::destNode() const
{
    return m_dest;
}

void Edge::adjust()
{
    if (!m_source)
        return;

    QLineF line;

    if(!m_dest)
        line=QLineF(mapFromItem(m_source,m_source->middlePoint()), mapFromItem(m_dest, 0,0));
    else
        line= QLineF(mapFromItem(m_source,m_source->middlePoint()), mapFromItem(m_dest, m_dest->middlePoint()));

    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.))
    {
        QPointF edgeOffset((line.dx() * (m_source->boundingRect().width()/2)) / length, (line.dy() * (m_source->boundingRect().height()/2)) / length);
        sourcePoint = line.p1() + edgeOffset;
        if(NULL!=m_dest)
        {
            edgeOffset.setX((line.dx() * m_dest->boundingRect().width()/2) / length);
            edgeOffset.setY((line.dy() * m_dest->boundingRect().height()/2) / length);
            destPoint = line.p2() - edgeOffset;
        }
    }
    else
    {
        sourcePoint = line.p1();
        if(NULL!=m_dest)
        {
            destPoint = line.p1();
        }
    }


}
void  Edge::setDestination(EdgableItems *destNode)
{
    m_dest = destNode;
    m_dest->addEdge(this);
    adjust();

}
void  Edge::setSource(EdgableItems *srcNode)
{
    m_source = srcNode;
    m_source->addEdge(this);
    adjust();

}
QRectF Edge::boundingRect() const
{
    if (!m_source)
        return QRectF();



    qreal penWidth = 1;
    qreal extra = (penWidth + m_arrowSize) / 2.0;

    QRectF  myRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),destPoint.y() - sourcePoint.y()));
            myRectF=myRectF.normalized();
            myRectF=myRectF.adjusted(-extra, -extra, extra, extra);

            if(myRectF.width()<m_textRect.width()*2)
            {
                myRectF.adjust(0,0,m_textRect.width(),0);
            }
            if(myRectF.height()<m_textRect.height())
            {
                myRectF.adjust(0,-m_textRect.height(),0,0);
            }

    return myRectF;
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem * option, QWidget *)
{
    if (!m_source)
        return;
    if(destPoint.isNull())
    {
        destPoint = sourcePoint;
    }




    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
         return;


        // Draw the line itself
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

    painter->drawText(line.pointAt(0.5),m_text);

    /// TEST of BOUNDING RECT
    if(option->state & QStyle::State_Selected)
    {
        qreal penWidth = 1;
        qreal extra = (penWidth + m_arrowSize) / 2.0;

        painter->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawRect(QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                                     destPoint.y() - sourcePoint.y()))
                       .normalized()
                       .adjusted(-extra, -extra, extra, extra));
    }
    /// END OF TEST of BOUNDING RECT




    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    // Draw the arrows
    double angle = ::acos(line.dx() / line.length());

    if (line.dy() >= 0)
    {
        angle = TwoPi - angle;
    }


    QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + Pi / 3) * m_arrowSize,
                                                  cos(angle + Pi / 3) * m_arrowSize);
    QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + Pi - Pi / 3) * m_arrowSize,
                                                  cos(angle + Pi - Pi / 3) * m_arrowSize);
    QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * m_arrowSize,
                                              cos(angle - Pi / 3) * m_arrowSize);
    QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * m_arrowSize,
                                              cos(angle - Pi + Pi / 3) * m_arrowSize);

    painter->setBrush(Qt::black);
    if((m_endkind == Edge::BOTH)||(m_endkind == Edge::END1))
    {
        painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
    }
    if((m_endkind == Edge::BOTH)||(m_endkind == Edge::END2))
    {
        painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
    }
}
void Edge::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event )
{
    if(NULL==m_proxyw)
    {
        m_proxyw = scene()->addWidget(m_tempedit);
    }
    m_proxyw->setPos(event->pos().x(),event->pos().y());

    //QGraphicsProxyWidget* tmp = scene()->addWidget(m_tempedit);
   //tmp->setPos(pos().x()-boundingRect().width(),pos().y()-boundingRect().height());
    m_tempedit->setText(m_text);
    m_tempedit->selectAll();
    m_tempedit->setFocus(Qt::OtherFocusReason);
    m_tempedit->show();

}
void Edge::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsObject::mousePressEvent(event);
}

void Edge::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsObject::mouseReleaseEvent(event);
}
void Edge::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsObject::mouseMoveEvent(event);
}
void Edge::setDestinationPoint(QPointF pos)
{
    destPoint = pos;
    update();
}
void Edge::setGrap(GraphWidget* graph)
{
   m_graph = graph;
}

void Edge::readFromData(QDataStream& in)
{
    in >> m_arrowSize;
    in >> m_text;
    setText(m_text);
    QString uuid;
    int i;
    in >> i;
    setKind((Edge::EndKind)i);
    in >>m_uuidSrc;

    in >>m_uuidDst;

    lookUpPoint();

    adjust();
    update();
}
void Edge::lookUpPoint()
{
    m_source= m_graph->getEdgableItemFromUuid(m_uuidSrc);
    m_dest=  m_graph->getEdgableItemFromUuid(m_uuidDst);
    if(NULL!=m_source)
    {
        m_source->addEdge(this);
    }
    if(NULL!=m_dest)
    {
        m_dest->addEdge(this);
    }
}

QString Edge::getText() const
{
    return m_text;
}

void Edge::writeToData(QDataStream& out)
{
    out << m_arrowSize;
    out << m_text;
    out << (int)m_endkind;
    out << m_source->getUuid();
    out << m_dest->getUuid();
}
void Edge::updatePainting()
{
   update();
}
void Edge::setText(QString t)
{
   m_text = t;
   QFont font;
   QFontMetrics info(font);
   m_textRect = info.boundingRect(m_text);
}
void Edge::setKind(Edge::EndKind kind)
{
    m_endkind = kind;
    updatePainting();
}

Edge::EndKind  Edge::getKind()
{
    return m_endkind;
}
QPainterPath  Edge::shape() const
{
    QPainterPath path;


    QLineF line(sourcePoint, destPoint);

    // Draw the arrows
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = TwoPi - angle;
    path.moveTo(sourcePoint);
    QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + Pi / 3) * m_arrowSize,
                                                  cos(angle + Pi / 3) * m_arrowSize);

    path.lineTo(sourceArrowP1);
    QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + Pi - Pi / 3) * m_arrowSize,
                                                  cos(angle + Pi - Pi / 3) * m_arrowSize);

    QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * m_arrowSize,
                                              cos(angle - Pi / 3) * m_arrowSize);
    QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * m_arrowSize,
                                              cos(angle - Pi + Pi / 3) * m_arrowSize);
    path.lineTo(destArrowP1);
    path.lineTo(destArrowP2);
    path.lineTo(sourceArrowP2);
    path.closeSubpath ();

    return path;
}
void Edge::setName(QString&)
{

}

QString Edge::getName() const
{
    return tr("Edge");
}
QPixmap Edge::getIcon() const
{
    QPixmap pixmap(30,20);
    QPainter painter(&pixmap);
    painter.fillRect(pixmap.rect(),Qt::white);
    QLineF line(0,10,30,10);

    int arrowSize = 4;



    // Draw the line itself
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(line);




    // Draw the arrows
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = TwoPi - angle;

    QPointF sourceArrowP1 = line.p1() + QPointF(sin(angle + Pi / 3) * arrowSize,
                                                  cos(angle + Pi / 3) * arrowSize);
    QPointF sourceArrowP2 = line.p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                                  cos(angle + Pi - Pi / 3) * arrowSize);
    QPointF destArrowP1 = line.p2() + QPointF(sin(angle - Pi / 3) * arrowSize,
                                              cos(angle - Pi / 3) * arrowSize);
    QPointF destArrowP2 = line.p2() + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
                                              cos(angle - Pi + Pi / 3) * arrowSize);

    painter.setBrush(Qt::black);
    if((m_endkind == Edge::BOTH)||(m_endkind == Edge::END1))
    {
        painter.drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
    }
    if((m_endkind == Edge::BOTH)||(m_endkind == Edge::END2))
    {
        painter.drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
    }



    painter.end();
    return pixmap;
}



EdgableItems* Edge::getDestination()
{
    return m_dest;
}

EdgableItems* Edge::getSource()
{
    return m_source;
}

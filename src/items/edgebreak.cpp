/***************************************************************************
 *	Copyright (C) 2011 by Renaud Guezennec                             *
 *   http://renaudguezennec.homelinux.org/accueil,3.html                   *
 *                                                                         *
 *   rmindmap is free software; you can redistribute it and/or modify  *
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
#include <QUuid>

#include <QPainter>

#include "items/edgebreak.h"


#include "edge.h"

EdgeBreak::EdgeBreak()
{
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(NoCache);
    m_id = QUuid::createUuid().toString();

}

void EdgeBreak::addEdge(Edge* edge)
{
    m_edgeList << edge;
    edge->adjust();
}
QRectF EdgeBreak::boundingRect() const
{
    QRectF r(0,0,10,10);
    return r;
}
QString EdgeBreak::getUuid()
{
    return m_id;
}

QPointF EdgeBreak::middlePoint()
{
    return boundingRect().center();
}

QString EdgeBreak::getName() const
{
    return QString();
}

QString EdgeBreak::getText() const
{
    return QString();
}

QPixmap EdgeBreak::getIcon() const
{
    return QPixmap();
}
void EdgeBreak::setText(QString)
{

}

void EdgeBreak::setName(QString&)
{

}
void EdgeBreak::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->fillRect(boundingRect(),Qt::black);
}
void EdgeBreak::readFromData(QDataStream& in)
{
    in >> m_id;
    QPointF center;
    in >> center;
    setPos(center);
}

void EdgeBreak::writeToData(QDataStream& out)
{
    out << m_id;
    out << pos();
}
QVariant EdgeBreak::itemChange(GraphicsItemChange change, const QVariant &value)
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
void EdgeBreak::updateEdges()
{
    foreach (Edge *edge, m_edgeList)
    {
        edge->adjust();
    }
}
void EdgeBreak::setGeometry(int w,int h)
{

}

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
#include "linkitem.h"

#include "geometry/linknode.h"

LinkItem::LinkItem()
{
    setFlag(QQuickItem::ItemHasContents, true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setWidth(280);
    setHeight(280);
}

LinkController::Direction LinkItem::direction() const
{
    return m_direction;
}

void LinkItem::setDirection(const LinkController::Direction& direction)
{
    if(direction == m_direction)
        return;
    m_direction= direction;
    emit directionChanged();
}

QPointF LinkItem::start() const
{
    return m_start;
}

void LinkItem::setStart(const QPointF& start)
{
    if(m_start == start)
        return;
    m_start= start;
    update();
    emit startChanged();
}

QPointF LinkItem::end() const
{
    return m_end;
}

void LinkItem::setEnd(const QPointF& end)
{
    if(end == m_end)
        return;
    m_end= end;
    emit endChanged();
    update();
}

PointList LinkItem::points() const
{
    return m_points;
}

void LinkItem::setPoints(const PointList& list)
{
    if(list == m_points)
        return;
    m_points= list;
    emit pointsChanged();
}

QRectF LinkItem::startBox() const
{
    return m_startBox;
}
QRectF LinkItem::endBox() const
{
    return m_endBox;
}
void LinkItem::setStartBox(QRectF rect)
{
    if(m_startBox == rect)
        return;
    m_startBox= rect;
    emit startBoxChanged();
}
void LinkItem::setEndBox(QRectF rect)
{
    if(rect == m_endBox)
        return;
    m_endBox= rect;
    emit endBoxChanged();
}

QColor LinkItem::color() const
{
    return m_color;
}

void LinkItem::setColor(QColor color)
{
    if(m_color == color)
        return;
    m_color= color;
    emit colorChanged();
    m_colorChanged= true;
    update();
}

void LinkItem::mousePressEvent(QMouseEvent* event)
{
    if(event->button() & Qt::LeftButton)
    {
        emit selected(true);
        event->accept();
    }

    // QQuickItem::mousePressEvent(event);
}

QSGNode* LinkItem::updatePaintNode(QSGNode* node, QQuickItem::UpdatePaintNodeData*)
{
    LinkNode* link= static_cast<LinkNode*>(node);
    if(!link)
    {
        link= new LinkNode();
        link->setColor(m_color);
    }
    if(m_colorChanged)
    {
        link->setColor(m_color);
        m_colorChanged= false;
    }

    // auto abox= m_startBox;
    // abox.moveTo(-m_startBox.width() / 2, -m_startBox.height() / 2);
    // auto bbox= m_endBox;

    // QPointF p2;

    // if(m_end.x() > m_start.x() && m_end.y() > m_start.y())
    // p2= m_end - m_start;
    // else
    //    p2= m_start - m_end;

    // auto hw= bbox.width() / 2;
    // auto hh= bbox.height() / 2;
    // bbox.setCoords(std::abs(p2.x()) - hw, std::abs(p2.y()) - hh, std::abs(p2.x()) + hw, std::abs(p2.y()) + hh);
    link->update(QRectF{0, 0, width(), height()}, m_orientation, m_startBox, m_endBox);
    return link;
}

const LinkController::Orientation& LinkItem::orientation() const
{
    return m_orientation;
}

void LinkItem::setOrientation(const LinkController::Orientation& newOrientation)
{
    if(m_orientation == newOrientation)
        return;
    m_orientation= newOrientation;
    emit orientationChanged();
    update();
}

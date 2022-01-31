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
#include "link.h"

#include "mindnode.h"
#include <QDebug>
#include <QRectF>
#include <cmath>

Link::Link(QObject* parent) : MindItem(MindItem::LinkType, parent)
{
    setText(tr("is linked"));
    connect(this, &Link::visibleChanged, this, [this]() {
        if(!m_end)
            return;
        m_end->setVisible(isVisible());
    });
}

void Link::setDirection(const Direction& direction)
{
    m_dir= direction;
}

Link::Direction Link::direction() const
{
    return m_dir;
}

PositionedItem* Link::start() const
{
    return m_start;
}

void Link::setStart(PositionedItem* start)
{
    if(start == m_start)
        return;

    if(m_start)
    {
        m_start->removeLink(this);
        disconnect(m_start, 0, this, 0);
    }

    m_start= start;

    if(nullptr != m_start)
    {
        m_start->addLink(this);
        connect(m_start, &MindNode::positionChanged, this, &Link::startPointChanged);
        connect(m_start, &MindNode::positionChanged, this, &Link::sizeChanged);
        connect(m_start, &MindNode::textChanged, this, [this]() {
            emit startBoxChanged();
            emit startPointChanged();
        });
    }
    emit startChanged();
}

void Link::setEnd(PositionedItem* end)
{
    if(end == m_end)
        return;

    if(m_end)
    {
        disconnect(m_end, 0, this, 0);
    }
    m_end= end;
    if(nullptr != m_end)
    {
        connect(m_end, &MindNode::positionChanged, this, &Link::endPointChanged);
        connect(m_end, &MindNode::positionChanged, this, &Link::sizeChanged);
        connect(m_end, &MindNode::textChanged, this, [this]() {
            qDebug() << m_end->boundingRect() << "text changed";
            emit endBoxChanged();
            emit endPointChanged();
        });
    }
    emit endChanged();
}

qreal Link::width() const
{
    if(!end() || !start())
        return 0.0;
    return end()->position().x() - start()->position().x();
}

qreal Link::height() const
{
    if(!end() || !start())
        return 0.0;
    return end()->position().y() - start()->position().y();
}

PositionedItem* Link::end() const
{
    return m_end;
}

QPointF Link::endPoint() const
{
    if(nullptr == m_end)
        return {};
    return m_end->centerPoint();
}

QPointF Link::startPoint() const
{
    if(nullptr == m_start)
        return {};
    return m_start->centerPoint();
}

void Link::computePosition()
{
    auto pos1= m_start->position();
    pos1.setY(pos1.y() + 50);
    m_end->setNextPosition(pos1, this);
}

float Link::getStiffness() const
{
    return m_stiffness;
}

void Link::setStiffness(float stiffness)
{
    m_stiffness= stiffness;
}

void Link::cleanUpLink()
{
    m_start->removeLink(this);
}

float Link::getLength() const
{
    auto length= 100.;
    auto rect1= m_start->boundingRect();
    auto diag1= std::sqrt(std::pow(rect1.width(), 2) + std::pow(rect1.height(), 2)) / 2;

    auto rect2= m_end->boundingRect();
    auto diag2= std::sqrt(std::pow(rect2.width(), 2) + std::pow(rect2.height(), 2)) / 2;

    auto realDiag= std::max(diag1, diag2);

    auto length1= static_cast<float>(length + 2 * realDiag);
    if(m_end == nullptr || m_start == nullptr)
        return length1;

    auto nodeCount= static_cast<int>(m_start->subLinks().size());

    auto endNodeCount= (m_end->subNodeCount() + nodeCount) / 3;
    auto length2= static_cast<float>(length * (1 + endNodeCount));

    return std::max(length1, length2);
}

const QRectF Link::endBox() const
{
    return m_end ? m_end->boundingRect() : QRectF{};
}

const QRectF Link::startBox() const
{
    return m_start ? m_start->boundingRect() : QRectF{};
}

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
#ifndef LINK_H
#define LINK_H

#include <QObject>
#include <QPointF>
#include <QPointer>
#include <QRectF>

#include "data/minditem.h"
//#include "data/positioneditem.h"

class PositionedItem;
class Link : public MindItem
{
    Q_OBJECT
    Q_PROPERTY(Direction direction READ direction WRITE setDirection NOTIFY directionChanged)

    Q_PROPERTY(PositionedItem* start READ start WRITE setStart NOTIFY startChanged)
    Q_PROPERTY(PositionedItem* end READ end WRITE setEnd NOTIFY endChanged)

    Q_PROPERTY(QPointF startPoint READ startPoint NOTIFY startPointChanged)
    Q_PROPERTY(QPointF endPoint READ endPoint NOTIFY endPointChanged)

    Q_PROPERTY(QRectF startBox READ startBox NOTIFY startBoxChanged)
    Q_PROPERTY(QRectF endBox READ endBox NOTIFY endBoxChanged)

    Q_PROPERTY(qreal width READ width NOTIFY sizeChanged)
    Q_PROPERTY(qreal height READ height NOTIFY sizeChanged)
public:
    enum Direction
    {
        StartToEnd,
        EndToStart,
        Both
    };
    Q_ENUM(Direction)
    explicit Link(QObject* parent= nullptr);

    Direction direction() const;
    PositionedItem* start() const;
    PositionedItem* end() const;
    QPointF endPoint() const;
    QPointF startPoint() const;

    float getLength() const;
    float getStiffness() const;
    void cleanUpLink();

    const QRectF endBox() const;
    const QRectF startBox() const;

    qreal width() const;
    qreal height() const;

public slots:
    void setStiffness(float stiffness);
    void setDirection(const Direction& direction);
    void setEnd(PositionedItem* end);
    void setStart(PositionedItem* start);
    void computePosition();

signals:
    void sizeChanged();
    void directionChanged();
    void startChanged();
    void endChanged();
    void startPointChanged();
    void endPointChanged();
    void startBoxChanged();
    void endBoxChanged();

private:
    Direction m_dir= StartToEnd;
    QPointer<PositionedItem> m_start;
    QPointer<PositionedItem> m_end;
    float m_stiffness= 400.0f;
};

#endif // LINK_H

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
#ifndef LINKITEM_H
#define LINKITEM_H

#include <QList>
#include <QQuickItem>

typedef QList<QPointF> PointList;

class LinkItem : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(Direction direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(QPointF start READ start WRITE setStart NOTIFY startChanged)
    Q_PROPERTY(QPointF end READ end WRITE setEnd NOTIFY endChanged)
    Q_PROPERTY(QRectF startBox READ startBox WRITE setStartBox NOTIFY startBoxChanged)
    Q_PROPERTY(QRectF endBox READ endBox WRITE setEndBox NOTIFY endBoxChanged)
    // may not be used
    Q_PROPERTY(PointList points READ points WRITE setPoints NOTIFY pointsChanged)

public:
    enum Direction
    {
        StartToEnd,
        EndToStart,
        Both
    };
    Q_ENUM(Direction)
    LinkItem();

    Direction direction() const;
    void setDirection(const Direction& direction);
    QPointF start() const;
    void setStart(const QPointF& start);
    QPointF end() const;
    void setEnd(const QPointF& end);

    PointList points() const;
    void setPoints(const PointList& list);

    QRectF startBox() const;
    QRectF endBox() const;
    void setStartBox(QRectF rect);
    void setEndBox(QRectF rect);

signals:
    void directionChanged();
    void startChanged();
    void endChanged();
    void pointsChanged();
    void startBoxChanged();
    void endBoxChanged();

protected:
    QSGNode* updatePaintNode(QSGNode*, UpdatePaintNodeData*) override;

private:
    QColor m_color= Qt::black;
    Direction m_direction= StartToEnd;
    QPointF m_start;
    QPointF m_end;
    QRectF m_startBox;
    QRectF m_endBox;
    PointList m_points;
};

#endif // LINKITEM_H

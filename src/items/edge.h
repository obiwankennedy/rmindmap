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
#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsObject>
#include <QLineEdit>

#include "items/genericmindmapitem.h"

class GenericMindMapItem;
class GraphWidget;

class Edge : public GenericMindMapItem
{
    Q_OBJECT

public:
    enum ArrowHead {BOTH,END1,END2,NONE};
    enum LineType {Straight, Curve};
    Edge(EdgableItem *sourceNode, EdgableItem *destNode);
    Edge(EdgableItem *sourceNode);
    Edge();

    EdgableItem *sourceNode() const;
    EdgableItem *destNode() const;
    QPainterPath shape() const;
    void setGrap(GraphWidget*);

    void adjust();

    enum { Type = UserType + 2 };
    int type() const { return Type; }


    QString getText() const;

    virtual void setName(QString&);
    virtual QString getName() const;


    void setKind(Edge::ArrowHead);
    Edge::ArrowHead getKind();

    void setDestination(EdgableItem *destNode);
    void setSource(EdgableItem *srcNode);


    EdgableItem* getDestination();
    EdgableItem* getSource();

    QString getDestId() const;

    void setDestinationPoint(QPointF);

    virtual void readFromData(QJsonObject&);
    virtual void writeToData(QJsonObject&);

    virtual QPixmap getIcon() const;
    void lookUpPoint();

    virtual void setGeometry(int w,int h);

public slots:
    void setText(QString);
    void updatePainting();



protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event );
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    
private: //function;
    void init();

private:
    EdgableItem *m_source, *m_dest;

    QPointF sourcePoint;
    QPointF destPoint;
    qreal m_arrowSize;
    QString m_uuidSrc;
    QString m_uuidDst;

   // GraphWidget* m_graph;

    QString m_text;
    QLineEdit* m_tempedit;
    QGraphicsProxyWidget* m_proxyw;
    QRect m_textRect;

    ArrowHead m_arrowHead;
    QPainterPath m_path;

    QPointF m_sourceTanPoint;
    QPointF m_destTanPoint;

    bool m_showTanPoints;


    LineType m_type;
};


#endif

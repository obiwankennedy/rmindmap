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
#ifndef PACKAGEITEM_H
#define PACKAGEITEM_H


#include "genericmindmapitem.h"

class GraphWidget;
class Edge;
class PackageItem : public EdgableItem
{
public:
    enum Border {STRONG,TINY,MIDDLE};
    PackageItem();

    QString getName() const;
    QString getText() const;
    QRectF boundingRect() const;
    void setTopLeft(QPointF);
    void setBottomRight(QPointF);
    void setBorder(PackageItem::Border);
    QRectF rect();

    virtual void readFromData(QJsonObject&);
    virtual void writeToData(QJsonObject&);
    void setGrap(GraphWidget*);



    virtual void readFromData(QJsonObject&,EdgableItems*,QGraphicsScene* m_scene);
    virtual void writeToData(QJsonObject&,EdgableItems*,QHash<QString,GenericMindMapItem*>* done);

    virtual QPixmap getIcon() const;
    virtual void addEdge(Edge* );
    virtual QPointF middlePoint();
    virtual void setGeometry(int w, int h);
    virtual void updateHW();

public slots:
    void setName(QString&);
    void setText(QString q);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
private:
    void updateEdges();
private:
    QString m_name;
    QString m_title;
//    GraphWidget* m_graph;
    QPointF m_topLeft;
    QPointF m_bottomRight;
    PackageItem::Border m_border;
    QList<Edge *> m_edgeList;
    quint64 m_w;
    quint64 m_h;
};

#endif // PACKAGEITEM_H

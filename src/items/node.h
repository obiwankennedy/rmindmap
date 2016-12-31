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
#ifndef NODE_H
#define NODE_H

#include <QGraphicsObject>
#include <QList>
#include <QLineEdit>

#include "items/genericmindmapitem.h"

class Edge;
class GraphWidget;
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE


class Node : public EdgableItem
{
    Q_OBJECT
public:
    Node(QObject* graphWidget = nullptr);

    void addEdge(Edge *edge);
    QList<Edge *> edges() const;

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QString getText() const;
    QString description() const;

    void setDescription(QString desc);

    virtual void readFromData(QJsonObject&);
    virtual void writeToData(QJsonObject&);

    QString getUuid();
    void setUuid(QString uuid);

    void updateEdges();
    virtual QString getName() const;

    virtual QPixmap getIcon() const;
    virtual QPointF middlePoint();
    //virtual void setGeometry(int w,int h);
    int getRadius() const;

 /*   GraphWidget* getGraph() const;
    void setGraph(GraphWidget * graph);*/

public slots:
    void setText(QString text);
    void setName(QString& t);
    void updatePainting();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event );
    virtual void keyPressEvent(QKeyEvent* event);

    
private:
    QList<Edge *> m_edgeList;
    QPointF newPos;
   // GraphWidget *m_graph;

    QString m_text;

 /*   qreal m_raduisv;
    qreal m_raduish;

    qreal m_raduisBigV;
    qreal m_raduisBigH;*/
    QRect m_textRect;
    static QRect m_minRect;

    QLineEdit* m_tempedit;

    QString m_id;
    QString m_description;
    QGraphicsProxyWidget* m_proxyw;

    QList<QGraphicsItem*> m_children;

};


#endif

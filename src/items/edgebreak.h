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
#ifndef EDGEBREAK_H
#define EDGEBREAK_H

#include "genericmindmapitem.h"

class EdgeBreak : public EdgableItem
{
public:
    EdgeBreak();

    virtual void addEdge(Edge* );

    virtual QString getUuid();
    virtual QPointF middlePoint();

    virtual QString getName() const;
    virtual QString getText() const;

    virtual QPixmap getIcon() const;

    QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);


    virtual void readFromData(QJsonObject&);
    virtual void writeToData(QJsonObject&);

    void setGeometry(int w,int h);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);



 public slots:
    virtual void setText(QString);
    virtual void setName(QString&);
private:
    void updateEdges();

    QString m_name;
    QString m_title;
    QString m_id;

    QPointF m_topLeft;
    QPointF m_bottomRight;

    QList<Edge *> m_edgeList;
};

#endif // EDGEBREAK_H

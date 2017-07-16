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
#include <QPainter>
#include <QDebug>
#include <QUuid>


#include "packageitem.h"

#include "graphwidget.h"

PackageItem::PackageItem() : m_name("Package")
{
    m_border = PackageItem::MIDDLE;
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    //setCacheMode(DeviceCoordinateCache);
    setCacheMode(NoCache);
    m_id = QUuid::createUuid().toString();
    m_topLeft.setX(0);
    m_topLeft.setY(0);
    updateHW();
}

void PackageItem::setName(QString& q)
{
    m_name = q;
}

QString PackageItem::getName() const
{
    return m_name;
}
void PackageItem::setText(QString q)
{
    m_title = q;
    update();
}

QString PackageItem::getText() const
{
    return m_title;
}
QRectF PackageItem::boundingRect() const
{
    QRectF r(0, 0, m_w,m_h);

  // qDebug() << "rect package" <<r;
    //rect.translate(lastAddedPackage->pos());


   // r.moveRight(r.width()/2);
    //r.moveTop(-r.height()/2);




//    r.setX(0);
//    r.setY(0);

    //qDebug() << "rect:" << r << " pos:"<< pos() << r;

    return r;
}
QRectF PackageItem::rect()
{
    return boundingRect();
}

void PackageItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(boundingRect().isNull())
        return;

    painter->drawRoundedRect(boundingRect().adjusted(0,0,-1,-1),20,20);

    if(option->state & QStyle::State_Selected)
    {
        painter->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawRect(boundingRect().adjusted(0,0,-1,-1));
    }

    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

        QRect r = option->fontMetrics.boundingRect(m_title);
    painter->drawText(boundingRect().x(),boundingRect().y(),r.width(),r.height(),Qt::AlignCenter, m_title);

}
void PackageItem::setTopLeft(QPointF p )
{
    m_topLeft=p;
    updateHW();


}

void PackageItem::setBottomRight(QPointF p)
{
    m_bottomRight=p;
    updateHW();
    update();
}
void PackageItem::setBorder(PackageItem::Border b)
{
    m_border= b;
}
void PackageItem::readFromData(QJsonObject& in)
{

  /*  in >> m_title;
    in >> m_id;
    QPointF pos;
    in >> pos;


    in >> m_topLeft;
    in >> m_bottomRight;
    int size;
    in >> size;
    QString uuid;

    for(int i=0;i<size;++i)
    {
        in >> uuid;

        EdgableItem*  node = m_graph->getEdgableItemFromUuid(uuid);
        if(NULL!=node)
        {
            node->setParentItem(this);
        }

    }
    setPos(pos);
    updateHW();*/

}
void PackageItem::writeToData(QJsonObject& out)
{
/*    out << m_title;
    out << m_id;
    out << pos();
    out << m_topLeft;
    out << m_bottomRight;
    out << childItems().size();
    foreach(QGraphicsItem* item,childItems())
    {
        if(NULL!=dynamic_cast<Node*>(item))
        {
            out << dynamic_cast<Node*>(item)->getUuid();

        }
    }*/
}
QVariant PackageItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemScenePositionHasChanged:
    case ItemPositionHasChanged:
        updateEdges();
        foreach(QGraphicsItem* item,childItems())
        {
            if(NULL!=dynamic_cast<Node*>(item))
            {
               dynamic_cast<Node*>(item)->updateEdges();

            }
        }
         break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}
void PackageItem::updateEdges()
{
    foreach (Edge *edge, m_edgeList)
    {
        edge->adjust();
    }
}
void PackageItem::writeToData(QDataStream& out)
{
    out << m_title;
    out << m_id;
    out << pos();
    out << m_topLeft;
    out << m_bottomRight;
    out << childItems().size();
    foreach(QGraphicsItem* item,childItems())
    {
        if(NULL!=dynamic_cast<Node*>(item))
        {
            out << dynamic_cast<Node*>(item)->getUuid();

        }
    }
}
#include <QJsonArray>
void PackageItem::writeToData(QJsonObject& obj,EdgableItems* parent,QHash<QString,GenericMindMapItem*>* done)
{
    if(!done->contains(m_id))
    {
        obj["type"] = "package";
    obj["title"] = m_title;
    obj["id"] = m_id;
    obj["x"] = pos().x();
    obj["y"] = pos().y();
    obj["tlx"] = m_topLeft.x();
    obj["tly"] = m_topLeft.y();
    obj["brx"] = m_bottomRight.x();
    obj["bry"] = m_bottomRight.y();

    QJsonArray children;
    for(QGraphicsItem* item : childItems())
    {
        if(NULL!=dynamic_cast<Node*>(item))
        {
            QJsonObject child;
            child["id"] =  dynamic_cast<Node*>(item)->getId();
            children.append(child["id"]);

        }
    }
     obj["children"] = children;

    QJsonArray edges;
    for(auto edge : m_edgeList)
    {

        if(edge->getSource() == this)
        {
            QJsonObject edgeJson;
            edge->writeToData(edgeJson,nullptr,done);
            edges.append(edgeJson);
        }
    }
    obj["edges"] = edges;
    }
/*    out << m_title;
    out << m_id;
    out << pos();
    out << m_topLeft;
    out << m_bottomRight;
    out << childItems().size();
    foreach(QGraphicsItem* item,childItems())
    {
        if(NULL!=dynamic_cast<Node*>(item))
        {
            out << dynamic_cast<Node*>(item)->getUuid();

        }
    }*/
}
QPixmap PackageItem::getIcon() const
{

    QPixmap pixmap(30,20);
    QPainter painter(&pixmap);
    painter.fillRect(pixmap.rect(),Qt::white);
    painter.drawRoundedRect(pixmap.rect().adjusted(0,0,-1,-1),2,2);



   return pixmap;
}

void PackageItem::addEdge(Edge* edge)
{
    m_edgeList << edge;
    edge->adjust();
}

QString PackageItem::getUuid()
{
    return m_id;
}
QPointF PackageItem::middlePoint()
{
    return boundingRect().center();
}
void PackageItem::updateHW()
{
    m_w = m_bottomRight.x()-pos().x();
    m_h = m_bottomRight.y()-pos().y();
}
void PackageItem::setGeometry(int w, int h)
{
    m_bottomRight.setX(m_topLeft.x()+w);
    m_bottomRight.setY(m_topLeft.y()+h);

    updateHW();
}

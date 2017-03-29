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
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QStyleOption>
#include <QUuid>
#include <QGraphicsProxyWidget>
#include <QPainterPath>
#include <QJsonArray>


#include "edge.h"
#include "node.h"
#include "graphwidget.h"
#include "selectionitem.h"

#define RADIUS 5
QRect Node::m_minRect = QRect(0,0,60,20);
Node::Node(QObject *graphWidget)
{
    m_proxyw = NULL;
    m_text="test";
    m_textRect = m_minRect;
    setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemSendsGeometryChanges|QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsFocusable);
    setZValue(-1);
    m_id = QUuid::createUuid().toString();


    m_tempedit = new QLineEdit();
    m_tempedit->setFrame(false);
    m_tempedit->setEnabled(true);
    m_tempedit->setFocus();
    connect(m_tempedit,SIGNAL(textChanged(QString)),this,SLOT(setText(QString)));
    connect(m_tempedit,SIGNAL(editingFinished()),m_tempedit,SLOT(hide()));
    connect(m_tempedit,SIGNAL(textChanged(QString)),this,SLOT(updatePainting()));

    SelectionItem* selector = new SelectionItem(this);
    //selector

    //m_children.append(selector);
}

void Node::addEdge(Edge *edge)
{
    m_edgeList << edge;
    edge->adjust();
}

QList<Edge *> Node::edges() const
{
    return m_edgeList;
}

QRectF Node::boundingRect() const
{
   // qDebug() << "bounding rect" << m_textRect;

    return m_textRect;
}

QPainterPath Node::shape() const
{
    return QGraphicsItem::shape();
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    QRect tmp=m_textRect;
    painter->save();

    QPainterPath myPainterPath;
    painter->setPen(Qt::NoPen);

     bool showChild = (QStyle::State_Selected & option->state);

    for(auto item : childItems())
    {
        item->setVisible(showChild);
    }


    myPainterPath.addRoundedRect(tmp,RADIUS,RADIUS);
    QBrush bgBrush = m_colorTheme->getBursh(0+tmp.width()/2,0,0+tmp.width()/2,tmp.height()*2);
    //mybrush.
    painter->fillPath(myPainterPath,bgBrush);
   // painter->drawRoundedRect(tmp,RADIUS,RADIUS);
    painter->restore();
    painter->save();

    painter->setPen(QPen(m_colorTheme->getTextColor()));
    painter->drawText(tmp,Qt::AlignCenter,m_text);
    painter->restore();
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change)
    {
    case ItemScenePositionHasChanged:
    case ItemPositionHasChanged:
         updateEdges();
         break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}
void Node::updateEdges()
{
    foreach (Edge *edge, m_edgeList)
    {
        edge->adjust();
    }
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Node mousePressEvent";
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event )
{
    Q_UNUSED(event);
    if(NULL==m_proxyw)
    {
        m_proxyw = scene()->addWidget(m_tempedit);
    }
    m_proxyw->setPos(pos().x()-boundingRect().width(),pos().y()-boundingRect().height());
    m_tempedit->setText(m_text);
    m_tempedit->selectAll();
    m_tempedit->setFocus(Qt::OtherFocusReason);
    m_tempedit->show();
    QGraphicsItem::mouseDoubleClickEvent(event);

}

/*GraphWidget *Node::getGraph() const
{
    return m_graph;
}

void Node::setGraph(GraphWidget *graph)
{
    m_graph = graph;
}*/

void Node::setText(QString text)
{
    m_text=text;
    QFont font;
    QFontMetrics info(font);

    m_textRect = info.boundingRect(QString(" %1 ").arg(m_text));
    m_textRect.setHeight(m_textRect.height()*1.5);

    if(m_minRect.height()*m_minRect.width()>m_textRect.height()*m_textRect.width())
    {
        m_textRect = m_minRect;
    }

    if(NULL!=m_stringManager)
    {
        m_stringManager->setValue(tr("%1_%2").arg(m_id).arg("text"),text);
    }
}

QString Node::getText() const
{
    if(NULL!=m_stringManager)
    {
        return m_stringManager->getValue(tr("%1_%2").arg(m_id).arg("text"));
    }
    return QString();
}
void  Node::updatePainting()
{
    update();
}
QString Node::description() const
{
    return m_description;
}

void Node::setDescription(QString desc)
{
    m_description = desc;
}

void Node::readFromData(QJsonObject& obj,EdgableItems* parent,QGraphicsScene* m_scene)
{
    m_text = obj["text"].toString();
    QString type = obj["type"].toString();
    m_id = obj["id"].toString();
    qreal x = obj["x"].toDouble();
    qreal y = obj["y"].toDouble();
    setPos(x,y);
    m_description = obj["description"].toString();
    QString colorId= obj["colorThemeId"].toString();

    m_colorTheme = PreferencesManager::getInstance()->getNodeColorTheme(colorId);

    QJsonArray edges = obj["edges"].toArray();
    for(auto e : edges)
    {
        Edge* edge = new Edge();
        QJsonObject edgeJson = e.toObject();
        edge->readFromData(edgeJson,this,m_scene);
        qDebug() << "add item to map";
        m_scene->addItem(edge);
        m_edgeList.append(edge);
    }
}
#include <QJsonArray>
void Node::writeToData(QJsonObject& obj,EdgableItems* parent,QHash<QString,GenericMindMapItem*>* done)
{
    if(!done->contains(m_id))
    {
        done->insert(m_id,this);
        obj["text"] =  m_text;
        obj["type"] = "node";
        obj["id"] =  m_id;
        obj["x"] =  pos().x();
        obj["y"] =  pos().y();
        obj["description"] =  m_description;
        obj["colorThemeId"] =  m_colorTheme->getId();

        QJsonArray edges;
        for(auto edge : m_edgeList)
        {
            if(edge->getSource() == this)
            {
                if(!done->contains(edge->getId()))
                {
                    QJsonObject edgeJson;
                    edge->writeToData(edgeJson,nullptr,done);
                    edges.append(edgeJson);
                }
            }
        }
        obj["edges"] = edges;

    }
 /*   out << m_text;
    QColor color;
    out << color;
    out << color;
    out << m_id;
    out << pos();
    out << m_description;*/
}
void Node::setName(QString&)
{

}

QString Node::getName() const
{
    return tr("Node");
}
QPixmap Node::getIcon() const
{
    QPixmap pixmap(30,20);
    QPainter painter(&pixmap);
    painter.fillRect(pixmap.rect(),Qt::white);
    painter.setPen(m_colorTheme->getTextColor());
    painter.drawEllipse(pixmap.rect().adjusted(0,0,-1,-1));
    painter.end();
    return pixmap;
}
QPointF Node::middlePoint()
{
    return m_textRect.center();
}

/*void Node::setGeometry(int w,int h)
{

}
*/
int Node::getRadius() const
{
    return RADIUS;
}

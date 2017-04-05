/***************************************************************************
    *   Copyright (C) 2016 by Renaud Guezennec                                *
    *   http://www.rolisteam.org/contact                                      *
    *                                                                         *
    *   rolisteam is free software; you can redistribute it and/or modify     *
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

#include "mindmap.h"

#include <QGraphicsSceneMouseEvent>
#include <QJsonArray>
#include <QDebug>

#include "items/edgebreak.h"
#include "items/edge.h"

MindMap::MindMap(QObject* parent)
    : QGraphicsScene(parent),m_currentPackage(nullptr)
{
    m_stringManager = new StringManager(this);
    m_preferences = PreferencesManager::getInstance();
}

void MindMap::mousePressEvent(QGraphicsSceneMouseEvent* event)
{

    if(event->modifiers() & Qt::ControlModifier)
    {
        qDebug()<< "test" << "inside if" << event->pos() << sceneRect();
        m_currentPackage = new PackageItem();
        m_currentPackage->setTopLeft(QPointF(0,0));
        m_currentPackage->setPos(event->scenePos());
        addItem(m_currentPackage);
    }
    else if(event->modifiers() & Qt::ShiftModifier)
    {
        QList<QGraphicsItem*> list = items(event->scenePos(),Qt::ContainsItemBoundingRect);
        qDebug()<< "test add break" << event->scenePos() << list.size();
        for(auto item : list)
        {
            qDebug() << item;
            Edge* edge = dynamic_cast<Edge*>(item);
            if(nullptr!=edge)
            {
                qDebug()<< "test" << "inside if" << event->pos() << sceneRect();
                EdgeBreak* edgeBreak = new EdgeBreak();
                EdgableItems* dest = edge->getDestination();
                edgeBreak->setPos(event->scenePos());
                addItem(edgeBreak);
                edge->setDestination(edgeBreak);
                Edge* edgeN = new Edge();
                edgeN->setSource(edgeBreak);
                edgeN->setDestination(dest);
                addItem(edgeN);
            }
        }
    }
    else
        QGraphicsScene::mousePressEvent(event);
}
void MindMap::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if((nullptr != m_currentPackage) && (event->modifiers() & Qt::ControlModifier))
    {
        //qDebug()<< "Move: " << event->pos();
            m_currentPackage->setBottomRight(event->scenePos());
    }
    else
        QGraphicsScene::mouseMoveEvent(event);
}
void MindMap::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    m_currentPackage = nullptr;
    QGraphicsScene::mouseReleaseEvent(event);
}

StringManager* MindMap::getStringManager() const
{
    return m_stringManager;
}

void MindMap::setStringManager(StringManager *stringManager)
{
    m_stringManager = stringManager;
}
void MindMap::addNodeAt(QPoint pos)
{
    Node* node = new Node(this);
    addItem(node);
    node->setPos(pos.x(),pos.y());
    node->setStringManager(m_stringManager);
    node->setColorTheme(m_preferences->getDefaultNodeColorTheme());
    node->setText("Root");
    m_roots.append(node);
}
void MindMap::writeToData(QJsonObject& root)
{
    QHash<QString,GenericMindMapItem*>* done = new QHash<QString,GenericMindMapItem*>();
    QJsonObject stringManager;
    m_stringManager->writeToData(stringManager,nullptr,done);
    root["srings"]=stringManager;
    QJsonArray children;
    for(auto node : m_roots)
    {
        QJsonObject obj;
        node->writeToData(obj,nullptr,done);
        children.append(obj);
    }
    root["children"]=children;
}
void MindMap::readFromData(QJsonObject& root)
{
    QJsonObject obj = root["strings"].toObject();
    m_stringManager->readFromData(obj,nullptr,this);

    QJsonArray children = root["children"].toArray();
    for(auto i : children)
    {
        QJsonObject child = i.toObject();
        QString type = child["type"].toString();
        if(type == "node")
        {
            Node* node = new Node();
            addItem(node);
            node->readFromData(child,nullptr,this);
            m_roots.append(node);
        }
    }
}

void MindMap::clearRoots()
{
    qDeleteAll(m_roots);
    m_roots.clear();
    clear();
}

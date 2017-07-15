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
#include <QJsonArray>

#include "mindmap.h"
#include <QDebug>


MindMap::MindMap(QObject* parent)
    : QGraphicsScene(parent)//,m_root(nullptr)
{
    m_preferences = PreferencesManager::getInstance();
}

void MindMap::readFromText(QTextStream& in)
{
    clear();
    m_roots.clear();

  //  m_root = nullptr;
    Node* current=nullptr;
    int currentD = 0;
    QList<Node*> m_listOfAncestor;
    Node* parent = nullptr;
    while(!in.atEnd())
    {
        //if(i==90)
         //   return;
        QString line=in.readLine();
        int depth = getDepth(line);
        QString data = line.trimmed();
//        qDebug() << "depth:" <<depth << "data:" <<data << "currentD:" <<currentD << "ancestor count:" <<m_listOfAncestor.size();
        if((depth==0)&&(data.isEmpty()))//&&(currentD==0)
        {
            continue;
        }
        else
        {
            if(nullptr!=current)
            {
                if(depth>currentD)//on level under
                {
                    parent = current;
                    //current = addNodeAt(QPoint(parent->childItems().count()*20,currentD*40));
                    current = addNodeAt(QPoint(currentD*100,parent->childItems().count()*40));
                    m_listOfAncestor.append(current);
                    currentD = depth;
                }
                else if(depth==currentD)// previous d == current d
                {
    //                qDebug() << "$$$$$$$$$$$$$$" << "parent:" << (parent==nullptr ? "null" : parent->getText());
                    m_listOfAncestor.removeLast();
                    //current = addNodeAt(QPoint(parent->childItems().count()*20,currentD*40));
                    current = addNodeAt(QPoint(currentD*100,parent->childItems().count()*40));
                    m_listOfAncestor.append(current);
                }
                else if(depth<currentD)//many level above
                {
                    while(m_listOfAncestor.size()>depth)
                        m_listOfAncestor.removeLast();

                    if(!m_listOfAncestor.isEmpty())
                    {
                        parent = m_listOfAncestor.last();
                    }
                    else
                    {
                        parent = nullptr;
                    }
             //       qDebug() << "////////////////" << "parent:" << (parent==nullptr ? "null" : parent->getText());
                    //current = addNodeAt(QPoint(0,currentD*40));
                    current = addNodeAt(QPoint(currentD*100,currentD*40));

                    if(nullptr==parent)
                    {
                        m_listOfAncestor.clear();
                        m_listOfAncestor.append(current);
                        m_roots.append(current);

                    }
                    m_listOfAncestor.append(current);
                    currentD = depth;
                }
                else
                {
           //         qDebug() << ";;;;;;;;;;;;;;;;;;;;;;;;;";
                }
                current->setText(data);
                linkItem(parent,current);
            }
            else
            {

                current = addNodeAt(QPoint(sceneRect().width()/2,sceneRect().height()/2));
                current->setText(data);
                m_roots.append(current);
                qDebug() << "add roots" << current;

                currentD = depth;
                parent = current;

                m_listOfAncestor.clear();
                m_listOfAncestor.append(current);
            }
        }
    }
}
void MindMap::linkItem(Node* first, Node* second)
{
    if(nullptr!=first)
    {
        second->setParentItem(first);
        Edge* edge = new Edge(first,second);
        edge->setKind(Edge::END2);
        edge->setText(" ");
        addItem(edge);
        edge->setParentItem(first);
        edge->adjust();
    }
}

int MindMap::getDepth(QString line)
{
   int i = 0;
   for(; i<line.size() && line[i] == ' ';i++)
   {}
   return i/2;
}

void MindMap::appendRoot(Node* root)
{
    m_roots.append(root);
}
Node* MindMap::addNodeAt(QPoint pos)
{
    Node* item = new Node(this);
    addItem(item);
    item->setPos(pos.x(),pos.y());
  //  item->setStringManager(m_stringManager);

    item->setColorTheme(m_preferences->getDefaultNodeColorTheme());
    item->setText("Root");

    return item;
}

StringManager *MindMap::getStringManager() const
{
    return m_stringManager;
}

void MindMap::setStringManager(StringManager *stringManager)
{
    m_stringManager = stringManager;
}
void MindMap::readFromData(QJsonObject& obj)
{
     clear();

    QJsonArray rootListJson = obj["roots"].toArray();

    for(auto it = rootListJson.begin(); it!=rootListJson.end(); ++it)
    {
        QJsonObject root = it->toObject();
        Node* node=new Node();
        addItem(node);
        node->readFromData(root);

        node->setPos(node->getX(),node->getY());
        m_roots.append(node);
    }
}

void MindMap::writeToData(QJsonObject& obj)
{
    QJsonArray rootListJson;
    for(Node* node : m_roots)
    {
        QJsonObject root;
        node->writeToData(root);
        rootListJson.append(root);
    }
    obj["roots"]=rootListJson;
}
#include <QPainter>
void MindMap::dumpMapInBipmap(QString file)
{
    clearSelection();
    setSceneRect(itemsBoundingRect());
    QImage image(sceneRect().size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    QPainter painter(&image);
    render(&painter);
    image.save(file);
}

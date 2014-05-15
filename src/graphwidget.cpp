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
#include <QtGui>
#include <QDebug>
#include <QAction>
#include <QMenu>
#include <QGraphicsItem>
#include <QSvgGenerator>
#include <QGraphicsSvgItem>

#include "graphwidget.h"
#include "edge.h"
#include "node.h"
#include "items/packageitem.h"


#include <math.h>

GraphWidget::GraphWidget(StringManager* stringManager,QWidget *parent)
    : QGraphicsView(parent),m_stringManager(stringManager)
{
    lastAddedPackage = NULL;
    m_currentEdgeBrush = NULL;
    initAction();
    m_scene = new QGraphicsScene(this);
    setContextMenuPolicy(Qt::DefaultContextMenu);
    connect(m_scene,SIGNAL(selectionChanged()),this,SLOT(selectionHasChanged()));

    m_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    m_scene->setSceneRect(0, 0, width(), height());
    setScene(m_scene);
    m_currentTool = MindToolBar::NONE;

    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setRenderHint(QPainter::Antialiasing);

    setWindowTitle(tr("MapMind"));
    m_pointList = new QList<QPoint>();
    m_begunArrow = true;
    m_begunGeo = true;
    lastAddedEdge=NULL;
    m_nodeList = new QList<Node*>();
    m_edgeList = new QList<Edge*>();
    m_packageList = new QList<PackageItem*>();


}

void GraphWidget::initAction()
{
    m_editAct = new QAction(tr("Edit"),this);
    m_editAct->setToolTip(tr("Edit item text"));
    //m_actionZoomIn->setIcon(QIcon(":/resources/icons/zoom-in-32.png"));
    connect(m_editAct,SIGNAL(triggered()),this,SLOT(editItem()));

    m_removeAct = new QAction(tr("Remove"),this);
    //m_removeAct->setIcon(QIcon(":/resources/icons/zoom-out-32.png"));
    m_removeAct->setToolTip(tr("Remove item"));
    connect(m_removeAct,SIGNAL(triggered()),this,SLOT(removeItem()));

    m_addAsBrushAct = new QAction(tr("Save Style"),this);
    //m_removeAct->setIcon(QIcon(":/resources/icons/zoom-out-32.png"));
    m_addAsBrushAct->setToolTip(tr("Save item as Brush"));

    connect(m_addAsBrushAct,SIGNAL(triggered()),this,SLOT(makeBrush()));

    m_boundItemsAct = new QAction(tr("Bound inner items"),this);
    connect(m_boundItemsAct,SIGNAL(triggered()),this,SLOT(attachedChild()));

}

void GraphWidget::removeItem()
{
    QList<QGraphicsItem*> items= m_scene->selectedItems();
    for(int i =0; ((i<items.size()));++i)
    {
        m_scene->removeItem(items[i]);
        if(NULL!=dynamic_cast<Node*>(items[i]))
        {
            m_nodeList->removeAll(dynamic_cast<Node*>(items[i]));
        }
        if(NULL!=dynamic_cast<Edge*>(items[i]))
        {
            m_edgeList->removeAll(dynamic_cast<Edge*>(items[i]));
        }
        if(NULL!=dynamic_cast<PackageItem*>(items[i]))
        {
            m_packageList->removeAll(dynamic_cast<PackageItem*>(items[i]));
        }
    }

}

void GraphWidget::editItem()
{

}
void GraphWidget::makeBrush()
{
    QList<QGraphicsItem*> items= m_scene->selectedItems();
    for(int i =0; ((i<items.size()));++i)
    {
        if(NULL!=dynamic_cast<Node*>(items[i]))
        {
            emit nodeAsBrush(dynamic_cast<Node*>(items[i]));
        }
    }
}

void  GraphWidget::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    if(m_scene->sceneRect().width()<event->size().width())
    {
        m_scene->setSceneRect(0, 0, width(), height());
    }
}


void GraphWidget::contextMenuEvent ( QContextMenuEvent * event )
{
    QMenu menu(this);
    menu.addAction(m_editAct);
    menu.addAction(m_removeAct);
    menu.addAction(m_addAsBrushAct);
    menu.addAction(m_boundItemsAct);

    m_point = event->pos();
    QGraphicsItem* item=itemAt(m_point);
    if(NULL!=dynamic_cast<PackageItem*>(item))
    {
        m_boundItemsAct->setEnabled(true);
    }
    else
    {
        m_boundItemsAct->setEnabled(false);
    }
    if(NULL!=dynamic_cast<Node*>(item))
    {
        m_addAsBrushAct->setEnabled(true);
    }
    else
    {
        m_addAsBrushAct->setEnabled(false);
    }


    menu.exec(event->globalPos());

    event->accept();

}
void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}




void GraphWidget::zoomIn()
{
    scaleView(qreal(1.2));
}

void GraphWidget::zoomOut()
{
    scaleView(1 / qreal(1.2));
}
void GraphWidget::manageArrow(QMouseEvent *event)
{
    if(m_begunArrow)
    {
        foreach(QGraphicsItem* item, items(event->pos()))
        {

            if(NULL!=dynamic_cast<EdgableItems*>(item))
            {
                lastAddedEdge = new Edge(dynamic_cast<EdgableItems*>(item));
                if(NULL!=m_currentEdgeBrush)
                {
                    lastAddedEdge->setKind(m_currentEdgeBrush->getKind());
                }

                emit itemHasBeenAdded(lastAddedEdge);
                lastAddedEdge->setGrap(this);
                m_scene->addItem(lastAddedEdge);
                m_edgeList->append(lastAddedEdge);
                m_begunArrow = false;
                setMouseTracking(true);
                lastAddedEdge->adjust();
            }
        }
    }
    else
    {

        foreach(QGraphicsItem* item, items(event->pos()))
        {

            if(NULL!=dynamic_cast<EdgableItems*>(item))
            {
                if(NULL!=lastAddedEdge)
                {

                    lastAddedEdge->setDestination(dynamic_cast<EdgableItems*>(item));
                    lastAddedEdge=NULL;
                    m_begunArrow = true;
                    setMouseTracking(false);
                }
            }
        }
    }
}

void GraphWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button()== Qt::RightButton)
        return ;

    if((MindToolBar::NONE == m_currentTool)||(MindToolBar::HANDLE == m_currentTool))
    {
        QGraphicsView::mousePressEvent(event);
    }
    else
    {
        if(MindToolBar::ADD_ITEM==m_currentTool)
        {
            if(Qt::LeftButton== event->button())
            {
                addNodeAt(event->pos());
            }
        }
        else if(MindToolBar::ARROW == m_currentTool)
        {
            manageArrow(event);
        }
        else if(MindToolBar::ADD_GEO == m_currentTool)
        {
            if(Qt::LeftButton== event->button())
            {
                addGeoAt(event->pos() /*event->posF()*/);
            }
        }
        else if(MindToolBar::ADD_BREAK == m_currentTool)
        {
            if(Qt::LeftButton== event->button())
            {
                addBreakAt(event->pos() /*event->posF()*/);
            }
        }
    }

}
void GraphWidget::addBreakAt(QPointF pos)
{
    QList<QGraphicsItem*> list = items(pos.toPoint());
    foreach(QGraphicsItem* item, list)
    {



        Edge* tmp2 = dynamic_cast<Edge*>(item);

        if(NULL!=tmp2)
        {

            EdgeBreak* tmpbreak = new EdgeBreak();
            tmpbreak->setPos(pos);

            Edge* newedge =  new Edge(tmpbreak,tmp2->getDestination());
            tmp2->setDestination(tmpbreak);

            Edge::EndKind endEdge1 = Edge::NONE;
            Edge::EndKind endEdge2 = Edge::NONE;


            if(tmp2->getKind()==Edge::BOTH)
            {
                endEdge1 = Edge::END1;
                endEdge2 = Edge::END2;
            }
            else if(tmp2->getKind()==Edge::END1)
            {
                endEdge1 = Edge::END1;
                endEdge2 = Edge::NONE;
            }
            else if(tmp2->getKind()==Edge::END2)
            {
                endEdge1 = Edge::NONE;
                endEdge2 = Edge::END2;
            }
            tmp2->setKind(endEdge1);
            newedge->setKind(endEdge2);



            m_scene->addItem(tmpbreak);
            m_scene->addItem(newedge);
        }

    }


}

void GraphWidget::addGeoAt(QPointF pos)
{
    if(m_begunGeo)
    {
        lastAddedPackage = new PackageItem();
        emit itemHasBeenAdded(lastAddedPackage);
        lastAddedPackage->setTopLeft(pos);
        //lastAddedPackage->setGrap(this);
        m_scene->addItem(lastAddedPackage);
        m_packageList->append(lastAddedPackage);
        m_begunGeo = false;
        setMouseTracking(true);

    }
    else
    {

        lastAddedPackage->setBottomRight(pos);

        m_begunGeo = true;
        setMouseTracking(false);

        QRectF rect = lastAddedPackage->rect();

        // qDebug() << "Add parent" << rect;
        foreach(QGraphicsItem* item,m_scene->items(rect))
        {
            if(NULL!=dynamic_cast<Node*>(item))
            {

                item->setParentItem(lastAddedPackage);
            }
        }
        lastAddedPackage=NULL;



    }
}

void GraphWidget::buildEdge()
{
    if((m_pointList->size()%2 == 0) && (m_pointList->size()>0))
    {
        for(int i =m_pointList->size()-1; i>0;i-=2)
        {
            EdgableItems* tmp1 = dynamic_cast<EdgableItems*>(itemAt(m_pointList->at(i-1)));
            EdgableItems* tmp2 = dynamic_cast<EdgableItems*>(itemAt(m_pointList->at(i)));

            if((NULL!=tmp1)&&(NULL!=tmp2)&&(tmp1!=tmp2))
            {
                Edge* tmp = new Edge(tmp1, tmp2);
                m_scene->addItem(tmp);
                emit itemHasBeenAdded(tmp);
                m_pointList->removeAt(i);
                m_pointList->removeAt(i-1);
            }
            if(NULL==tmp1)
            {
                m_pointList->removeAt(i-1);
            }
            if(NULL==tmp2)
            {
                m_pointList->removeAt(i);
            }
        }
    }
}

void GraphWidget::setCurrentNodeBrush(Node* node)
{
    m_currentNodeBrush = node;
}
void GraphWidget::addNodeAt(QPoint pos)
{
    Node* node = new Node(this);
    emit itemHasBeenAdded(node);
    m_scene->addItem(node);
    m_nodeList->append(node);
    node->setPos(pos.x(),pos.y());
    node->setStringManager(m_stringManager);
    node->setColor(m_currentNodeBrush->color());
    node->setBgColor(m_currentNodeBrush->bgColor());
    node->setText(m_currentNodeBrush->getText());


}
void  GraphWidget::setCurrentTool(MindToolBar::MINDTOOL tool)
{
    m_currentTool = tool;
    m_pointList->clear();
}

void GraphWidget::mouseMoveEvent(QMouseEvent* event)
{
    if(NULL!=lastAddedEdge)
    {
        lastAddedEdge->setDestinationPoint(event->pos() /*event->localPos()*/);
    }
    else if(NULL!=lastAddedPackage)
    {
        lastAddedPackage->setBottomRight(event->pos() /* event->localPos()*/);
    }
    QGraphicsView::mouseMoveEvent(event);
}
void GraphWidget::readFromData(QDataStream& out)
{
    qreal w,h;
    out >> w;
    out >> h;

    int size;
    out >> size;
    Node* node=NULL;
    for(int i=0; i<size;++i)
    {
        node = new Node(this);
        node->setStringManager(m_stringManager);
        emit itemHasBeenAdded(node);
        node->readFromData(out);
        m_scene->addItem(node);
        m_nodeList->append(node);
    }
    out >> size;

    Edge* edge=NULL;
    for(int i=0; i<size;++i)
    {
        edge = new Edge();
        emit itemHasBeenAdded(edge);
        edge->setGrap(this);
        edge->readFromData(out);
        m_scene->addItem(edge);
        m_edgeList->append(edge);
    }

    out >> size;
    PackageItem* pack=NULL;
    for(int i=0; i<size;++i)
    {
        pack = new PackageItem();
        pack->setGrap(this);
        emit itemHasBeenAdded(pack);
        pack->readFromData(out);
        m_scene->addItem(pack);
        m_packageList->append(pack);
    }

    foreach(Edge* edge, *m_edgeList )
    {
        edge->lookUpPoint();
    }

    m_scene->setSceneRect(0,0,w,h);
}

void GraphWidget::writeToData(QDataStream& in)
{
    in << m_scene->sceneRect().width();
    in << m_scene->sceneRect().height();
    in << m_nodeList->size();
    for(int i=0;i<m_nodeList->size();i++)
    {
        m_nodeList->at(i)->writeToData(in);
    }

    in << m_edgeList->size();
    for(int i=0;i<m_edgeList->size();i++)
    {
        m_edgeList->at(i)->writeToData(in);
    }

    in << m_packageList->size();
    for(int i=0;i<m_packageList->size();i++)
    {
        m_packageList->at(i)->writeToData(in);
    }

}
EdgableItems* GraphWidget::getEdgableItemFromUuid(QString id)
{
    EdgableItems* tmp = getNodeFromUuid(id);
    if(NULL==tmp)
    {
        return getPackageFromUuid(id);
    }

    return tmp;
}

Node* GraphWidget::getNodeFromUuid(QString id)
{
    for(int i=0;i<m_nodeList->size();i++)
    {
        if(m_nodeList->at(i)->getUuid() == id)
        {
            return m_nodeList->at(i);
        }
    }
    return NULL;
}
PackageItem* GraphWidget::getPackageFromUuid(QString id)
{
    for(int i=0;i<m_packageList->size();i++)
    {
        if(m_packageList->at(i)->getUuid() == id)
        {
            return m_packageList->at(i);
        }
    }
    return NULL;
}
void GraphWidget::attachedChild()
{
    PackageItem* tmp = dynamic_cast<PackageItem*>(itemAt(m_point));

    if(tmp!=NULL)
    {
        qDebug() << tmp->boundingRect() << tmp->pos();
        foreach(QGraphicsItem* item,m_scene->items(tmp->boundingRect().translated(tmp->pos()),Qt::IntersectsItemBoundingRect))
        {
            //qDebug()<< item->boundingRect() << item->pos();
            if(NULL!=dynamic_cast<Node*>(item))
            {
                Node* node = dynamic_cast<Node*>(item);

                PackageItem* tmp2 = dynamic_cast<PackageItem*>(item->parentItem());

                if(tmp2!=tmp)
                {
                    item->setParentItem(tmp);
                    item->setPos(item->pos().x()-tmp->pos().x(),item->pos().y()-tmp->pos().y());
                }
            }
            if(NULL!=dynamic_cast<Edge*>(item))
            {
                //item->setParentItem(tmp);
                qDebug() << dynamic_cast<Edge*>(item)->getText();

            }
        }
    }
}

void GraphWidget::selectionHasChanged()
{
    QList<QGraphicsItem*> items= m_scene->selectedItems();
    if(items.isEmpty())
    {
        emit selectionIsEmpty();
    }
    else
    {
        bool unfound=true;
        for(int i =0; ((i<items.size())&&(unfound));++i)
        {
            if(NULL!=dynamic_cast<Node*>(items[i]))
            {
                emit currentNode(dynamic_cast<Node*>(items[i]));
            }
            else if(NULL!=dynamic_cast<Edge*>(items[i]))
            {
                emit currentEdge(dynamic_cast<Edge*>(items[i]));
            }
            else if(NULL!=dynamic_cast<PackageItem*>(items[i]))
            {
                emit currentPackage(dynamic_cast<PackageItem*>(items[i]));
            }

        }
    }
}
void GraphWidget::cleanScene()
{
    m_scene->clear();
    m_nodeList->clear();
    m_edgeList->clear();
    m_packageList->clear();
}
void GraphWidget::dumpMapInSvg(QString file)
{
    QSvgGenerator svgGen;

    svgGen.setFileName(file);
    svgGen.setSize(m_scene->sceneRect().size().toSize());
    QRectF viewBox = m_scene->sceneRect();
    //viewBox.setX();
    svgGen.setViewBox(viewBox);
    svgGen.setTitle(tr("Export Map in SVG"));
    svgGen.setDescription(tr("the mind map is rendering in the svg format"));


    QPainter painter;
    painter.begin(&svgGen );
    painter.setRenderHint(QPainter::Antialiasing);
    m_scene->render( &painter,m_scene->sceneRect(),mapToScene(viewport()->rect()).boundingRect());
    painter.end();
}

void GraphWidget::dumpMapInBipmap(QString file)
{
    m_scene->clearSelection();
    m_scene->setSceneRect(m_scene->itemsBoundingRect());
    QImage image(m_scene->sceneRect().size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    QPainter painter(&image);
    m_scene->render(&painter);
    image.save(file);
}
void GraphWidget::setCurrentEdgeBrush(Edge* edge)
{
    m_currentEdgeBrush = edge;
    setCurrentTool(MindToolBar::ARROW);
}
void GraphWidget::removeGenericMindMapItem(GenericMindMapItem* item)
{
    m_scene->removeItem(item);


    Node* node = dynamic_cast<Node*>(item);
    if(NULL!=node)
    {
        m_nodeList->removeAll(node);
    }

    Edge* edge = dynamic_cast<Edge*>(item);
    if(NULL!=edge)
    {
        m_edgeList->removeAll(edge);
    }

    PackageItem* pkg = dynamic_cast<PackageItem*>(item);
    if(NULL!=pkg)
    {
        m_packageList->removeAll(pkg);
    }

    EdgeBreak* breaks = dynamic_cast<EdgeBreak*>(item);
    if(NULL!=breaks)
    {
        m_edgeBreakList->removeAll(breaks);
    }

}

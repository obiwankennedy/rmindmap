/***************************************************************************
 *	Copyright (C) 2011 by Renaud Guezennec                                 *
 *   http://renaudguezennec.homelinux.org/accueil,3.html                   *
 *                                                                         *
 *   rmindmap is free software; you can redistribute it and/or modify      *
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
#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>



#include "mindtoolbar.h"
#include "serializable.h"
#include "items/edgebreak.h"
#include "stringmanager/stringmanager.h"
#include "preferences/preferencesmanager.h"

#include "mindmap.h"

class Node;
class QAction;
class PackageItem;
/**
 * @brief The GraphWidget class
 */
class GraphWidget : public QGraphicsView,public Serialisable
{
    Q_OBJECT

public:
    GraphWidget(StringManager* stringManager,QWidget *parent = 0);

    virtual void readFromData(QDataStream&);
    virtual void writeToData(QDataStream&);


    void dumpMapInSvg(QString file);
    void dumpMapInBipmap(QString file);

    void cleanScene();
    EdgableItem* getEdgableItemFromUuid(QString id);

    void changeSceneSize(int w,int h);

signals:
    void currentNode(GenericMindMapItem*);
    void currentEdge(GenericMindMapItem*);
    void currentPackage(GenericMindMapItem*);

    void selectionIsEmpty();
    void nodeAsBrush(Node*);
    /**
     * @brief itemHasBeenAdded
     * @param p
     */
    void itemHasBeenAdded(GenericMindMapItem* p);
    /**
     * @brief itemHasBeenDeleted
     */
    void itemHasBeenDeleted(GenericMindMapItem* p);


public slots:
    void zoomIn();
    void zoomOut();

 //   void setCurrentNodeBrush(Node*);
    void setCurrentEdgeBrush(Edge*);
    void setCurrentTool(MindToolBar::MINDTOOL);

    void selectionHasChanged();

    /**
     * @brief removeItem
     * @param item
     */
    void removeGenericMindMapItem(GenericMindMapItem* item);


protected:
    void mousePressEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void scaleView(qreal scaleFactor);
    void mouseMoveEvent(QMouseEvent* event);
    void contextMenuEvent ( QContextMenuEvent * event );
    void initAction();

    void mouseReleaseEvent(QMouseEvent *event);
private slots:
    void removeItem();
    void editItem();
    void makeBrush();
    void attachedChild();

private://functions
    void addNodeAt(QPoint pos);
    void buildEdge();
    void manageArrow(QMouseEvent *event);
    void addGeoAt(QPointF pos);
    void addBreakAt(QPointF pos);
    Node* getNodeFromUuid(QString);
    PackageItem* getPackageFromUuid(QString id);
    EdgeBreak* getBreakFromUuid(QString id);


private:
    ColorTheme* m_defaultNodeBrush;
    ColorTheme* m_defaultEdgeBrush;
    MindMap* m_scene;

    bool m_begunArrow;
    bool m_begunGeo;

    MindToolBar::MINDTOOL m_currentTool;
    QList<QPoint>* m_pointList;

    QList<Node*>* m_nodeList;
    QList<Edge*>* m_edgeList;
    QList<PackageItem*>* m_packageList;
    QList<EdgeBreak*>* m_edgeBreakList;

    Edge* m_lastAddedEdge;
    PackageItem* lastAddedPackage;


    QAction* m_removeAct;
    QAction* m_editAct;
    QAction* m_addAsBrushAct;
    QAction* m_boundItemsAct;

    QPoint m_point;
    QPoint m_lastPoint;
    StringManager* m_stringManager;
};

#endif

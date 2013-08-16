#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>



#include "mindtoolbar.h"
#include "serializable.h"
#include "items/edgebreak.h"

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
    GraphWidget(QWidget *parent = 0);

    virtual void readFromData(QDataStream&);
    virtual void writeToData(QDataStream&);


    void dumpMapInSvg(QString file);
    void dumpMapInBipmap(QString file);

    void cleanScene();



    EdgableItems* getEdgableItemFromUuid(QString id);

signals:
    void currentNode(Node*);
    void currentEdge(Edge*);
    void currentPackage(PackageItem*);

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

    void setCurrentNodeBrush(Node*);
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


private:
    Node* m_currentNodeBrush;
    Edge* m_currentEdgeBrush;
    QGraphicsScene* m_scene;

    bool m_begunArrow;
    bool m_begunGeo;

    MindToolBar::MINDTOOL m_currentTool;
    QList<QPoint>* m_pointList;

    QList<Node*>* m_nodeList;
    QList<Edge*>* m_edgeList;
    QList<PackageItem*>* m_packageList;
    QList<EdgeBreak*>* m_edgeBreakList;

    Edge* lastAddedEdge;
    PackageItem* lastAddedPackage;


    QAction* m_removeAct;
    QAction* m_editAct;
    QAction* m_addAsBrushAct;
    QAction* m_boundItemsAct;

    QPoint m_point;
};

#endif

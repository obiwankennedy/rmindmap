#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsObject>
#include <QLineEdit>

#include "items/genericmindmapitem.h"

class GenericMindMapItem;
class GraphWidget;

class Edge : public GenericMindMapItem
{
    Q_OBJECT

public:
    enum EndKind {BOTH,END1,END2,NONE};
    Edge(EdgableItems *sourceNode, EdgableItems *destNode);
    Edge(EdgableItems *sourceNode);
    Edge();

    EdgableItems *sourceNode() const;
    EdgableItems *destNode() const;
    QPainterPath shape() const;
    void setGrap(GraphWidget*);

    void adjust();

    enum { Type = UserType + 2 };
    int type() const { return Type; }


    QString getText() const;

    virtual void setName(QString&);
    virtual QString getName() const;


    void setKind(Edge::EndKind);
    Edge::EndKind getKind();

    void setDestination(EdgableItems *destNode);
    void setSource(EdgableItems *srcNode);


    EdgableItems* getDestination();
    EdgableItems* getSource();

    void setDestinationPoint(QPointF);

    virtual void readFromData(QDataStream&);
    virtual void writeToData(QDataStream&);

    virtual QPixmap getIcon() const;
    void lookUpPoint();

    virtual void setGeometry(int w,int h);

public slots:
    void setText(QString);
    void updatePainting();



protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event );
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    
private: //function;
    void init();

private:
    EdgableItems *m_source, *m_dest;

    QPointF sourcePoint;
    QPointF destPoint;
    qreal m_arrowSize;
    QString m_uuidSrc;
    QString m_uuidDst;

    GraphWidget* m_graph;

    QString m_text;
    QLineEdit* m_tempedit;
    QGraphicsProxyWidget* m_proxyw;
    QRect m_textRect;

    EndKind m_endkind;

    QPointF m_sourceTanPoint;
    QPointF m_destTanPoint;



};


#endif

#ifndef PACKAGEITEM_H
#define PACKAGEITEM_H


#include "genericmindmapitem.h"

class GraphWidget;
class Edge;
class PackageItem : public EdgableItems
{
public:
    enum Border {STRONG,TINY,MIDDLE};
    PackageItem();



    QString getName() const;

    QString getText() const;
    QRectF boundingRect() const;

    void setTopLeft(QPointF);
    void setBottomRight(QPointF);

    void setBorder(PackageItem::Border);

    QRectF rect();

    virtual void readFromData(QDataStream&);
    virtual void writeToData(QDataStream&);
    void setGrap(GraphWidget*);

    virtual QPixmap getIcon() const;

    virtual void addEdge(Edge* );

    virtual QString getUuid();

    virtual QPointF middlePoint();

public slots:
    void setName(QString&);
    void setText(QString q);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);


private:
    void updateEdges();

private:
    QString m_name;
    QString m_title;
    QString m_id;
    GraphWidget* m_graph;


    QPointF m_topLeft;
    QPointF m_bottomRight;
    PackageItem::Border m_border;
    QList<Edge *> m_edgeList;
};

#endif // PACKAGEITEM_H

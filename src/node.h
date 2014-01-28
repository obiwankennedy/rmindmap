#ifndef NODE_H
#define NODE_H

#include <QGraphicsObject>
#include <QList>
#include <QLineEdit>

#include "items/genericmindmapitem.h"

class Edge;
class GraphWidget;
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE


class Node : public EdgableItems
{
    Q_OBJECT
public:
    Node(GraphWidget *graphWidget);

    void addEdge(Edge *edge);
    QList<Edge *> edges() const;

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QString getText() const;
    QColor color() const;
    QColor bgColor() const;
    QString description() const;

    void setDescription(QString desc);

    virtual void readFromData(QDataStream&);
    virtual void writeToData(QDataStream&);

    QString getUuid();
    void setUuid(QString uuid);

    void updateEdges();
    virtual QString getName() const;

    virtual QPixmap getIcon() const;
    virtual QPointF middlePoint();
    virtual void setGeometry(int w,int h);

public slots:
    void setText(QString text);
    void setName(QString& t);
    void setColor(QColor);
    void setBgColor(QColor);
    void updatePainting();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event );

    
private:
    QList<Edge *> edgeList;
    QPointF newPos;
    GraphWidget *graph;
    QColor m_bgcolor;
    QColor m_color;

    QString m_text;

    qreal m_raduisv;
    qreal m_raduish;

    qreal m_raduisBigV;
    qreal m_raduisBigH;
    QRect m_textRect;

    QLineEdit* m_tempedit;

    QString m_id;
    QString m_description;
    QGraphicsProxyWidget* m_proxyw;
};


#endif

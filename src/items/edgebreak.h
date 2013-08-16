#ifndef EDGEBREAK_H
#define EDGEBREAK_H

#include "genericmindmapitem.h"

class EdgeBreak : public EdgableItems
{
public:
    EdgeBreak();

    virtual void addEdge(Edge* );

    virtual QString getUuid();
    virtual QPointF middlePoint();

    virtual QString getName() const;
    virtual QString getText() const;

    virtual QPixmap getIcon() const;

    QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);


    virtual void readFromData(QDataStream&);
    virtual void writeToData(QDataStream&);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);



 public slots:
    virtual void setText(QString);
    virtual void setName(QString&);
private:
    void updateEdges();

    QString m_name;
    QString m_title;
    QString m_id;

    QPointF m_topLeft;
    QPointF m_bottomRight;

    QList<Edge *> m_edgeList;
};

#endif // EDGEBREAK_H

#ifndef GENERICMINDMAPITEM_H
#define GENERICMINDMAPITEM_H

#include <QGraphicsItem>


#include "serializable.h"


class GenericMindMapItem : public QGraphicsObject,public Serialisable
{
public:
   virtual QString getName() const = 0;
   virtual QString getText() const = 0;

   virtual QPixmap getIcon() const =0;




public slots:
   virtual void setText(QString) = 0;
   virtual void setName(QString&) = 0;


};
class Edge;
class EdgableItems : public GenericMindMapItem
{
public:
    virtual void addEdge(Edge* ) = 0;

    virtual QString getUuid() = 0;
    virtual QPointF middlePoint() = 0;
};

#endif // GENERICMINDMAPITEM_H

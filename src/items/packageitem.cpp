#include <QPainter>
#include <QDebug>
#include <QUuid>


#include "packageitem.h"

#include "graphwidget.h"

PackageItem::PackageItem() : m_name("Package")
{
    m_border = PackageItem::MIDDLE;
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    //setCacheMode(DeviceCoordinateCache);
    setCacheMode(NoCache);
    m_id = QUuid::createUuid().toString();
    m_topLeft.setX(0);
    m_topLeft.setY(0);
}

void PackageItem::setName(QString& q)
{
    m_name = q;
}

QString PackageItem::getName() const
{
    return m_name;
}
void PackageItem::setText(QString q)
{
    m_title = q;
    update();
}

QString PackageItem::getText() const
{
    return m_title;
}
QRectF PackageItem::boundingRect() const
{
    QRectF r(0, 0, m_bottomRight.x()-m_topLeft.x(),m_bottomRight.y()-m_topLeft.y());
    //rect.translate(lastAddedPackage->pos());


   // r.moveRight(r.width()/2);
    //r.moveTop(-r.height()/2);




//    r.setX(0);
//    r.setY(0);

    //qDebug() << "rect:" << r << " pos:"<< pos() << r;

    return r;
}
QRectF PackageItem::rect()
{
    return boundingRect();
}

void PackageItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(boundingRect().isNull())
        return;

    painter->drawRoundedRect(boundingRect().adjusted(0,0,-1,-1),20,20);

    if(option->state & QStyle::State_Selected)
    {
        painter->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawRect(boundingRect().adjusted(0,0,-1,-1));
    }

    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

        QRect r = option->fontMetrics.boundingRect(m_title);
    painter->drawText(boundingRect().x(),boundingRect().y(),r.width(),r.height(),Qt::AlignCenter, m_title);

}
void PackageItem::setTopLeft(QPointF p )
{
    m_topLeft=p;
    //setPos(p);


}
void  PackageItem::setGrap(GraphWidget* g)
{
    m_graph=g;
}

void PackageItem::setBottomRight(QPointF p)
{
    m_bottomRight=p;
    /*m_bottomRight.setX(m_bottomRight.x()-pos().x());
    m_bottomRight.setY(m_bottomRight.y()-pos().y());*/
    update();
}
void PackageItem::setBorder(PackageItem::Border b)
{
    m_border= b;
}
void PackageItem::readFromData(QDataStream& in)
{

    in >> m_title;
    in >> m_id;
    QPointF pos;
    in >> pos;


    in >> m_topLeft;
    in >> m_bottomRight;
    int size;
    in >> size;
    QString uuid;

    for(int i=0;i<size;++i)
    {
        in >> uuid;

        EdgableItems*  node = m_graph->getEdgableItemFromUuid(uuid);
        if(NULL!=node)
        {
            node->setParentItem(this);
        }

    }
    setPos(pos);

}
QVariant PackageItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemScenePositionHasChanged:
    case ItemPositionHasChanged:
        updateEdges();
        foreach(QGraphicsItem* item,childItems())
        {
            if(NULL!=dynamic_cast<Node*>(item))
            {
               dynamic_cast<Node*>(item)->updateEdges();

            }
        }
         break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}
void PackageItem::updateEdges()
{
    foreach (Edge *edge, m_edgeList)
    {
        edge->adjust();
    }
}
void PackageItem::writeToData(QDataStream& out)
{
    out << m_title;
    out << m_id;
    out << pos();
    out << m_topLeft;
    out << m_bottomRight;
    out << childItems().size();
    foreach(QGraphicsItem* item,childItems())
    {
        if(NULL!=dynamic_cast<Node*>(item))
        {
            out << dynamic_cast<Node*>(item)->getUuid();

        }
    }
}
QPixmap PackageItem::getIcon() const
{

    QPixmap pixmap(30,20);
    QPainter painter(&pixmap);
    painter.fillRect(pixmap.rect(),Qt::white);
    painter.drawRoundedRect(pixmap.rect().adjusted(0,0,-1,-1),2,2);



   return pixmap;
}

void PackageItem::addEdge(Edge* edge)
{
    m_edgeList << edge;
    edge->adjust();
}

QString PackageItem::getUuid()
{
    return m_id;
}
QPointF PackageItem::middlePoint()
{
    return boundingRect().center();
}

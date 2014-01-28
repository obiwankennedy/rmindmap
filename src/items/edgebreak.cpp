#include <QUuid>

#include <QPainter>

#include "items/edgebreak.h"


#include "edge.h"

EdgeBreak::EdgeBreak()
{
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(NoCache);
    m_id = QUuid::createUuid().toString();

}

void EdgeBreak::addEdge(Edge* edge)
{
    m_edgeList << edge;
    edge->adjust();
}
QRectF EdgeBreak::boundingRect() const
{
    QRectF r(0,0,10,10);
    return r;
}
QString EdgeBreak::getUuid()
{
    return m_id;
}

QPointF EdgeBreak::middlePoint()
{
    return boundingRect().center();
}

QString EdgeBreak::getName() const
{
    return QString();
}

QString EdgeBreak::getText() const
{
    return QString();
}

QPixmap EdgeBreak::getIcon() const
{
    return QPixmap();
}
void EdgeBreak::setText(QString)
{

}

void EdgeBreak::setName(QString&)
{

}
void EdgeBreak::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->fillRect(boundingRect(),Qt::black);
}
void EdgeBreak::readFromData(QDataStream& in)
{
    in >> m_id;
    QPointF center;
    in >> center;
    setPos(center);
}

void EdgeBreak::writeToData(QDataStream& out)
{
    out << m_id;
    out << pos();
}
QVariant EdgeBreak::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemScenePositionHasChanged:
    case ItemPositionHasChanged:
         updateEdges();
         break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}
void EdgeBreak::updateEdges()
{
    foreach (Edge *edge, m_edgeList)
    {
        edge->adjust();
    }
}
void EdgeBreak::setGeometry(int w,int h)
{

}

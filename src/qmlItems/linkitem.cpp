#include "linkitem.h"

#include "geometry/linknode.h"

LinkItem::LinkItem()
{
    setFlag(QQuickItem::ItemHasContents, true);
    setWidth(80);
    setHeight(80);
}

LinkItem::Direction LinkItem::direction() const
{
    return m_direction;
}

void LinkItem::setDirection(const LinkItem::Direction& direction)
{
    if(direction == m_direction)
        return;
    m_direction= direction;
    emit directionChanged();
}

QPointF LinkItem::start() const
{
    return m_start;
}

void LinkItem::setStart(const QPointF& start)
{
    if(m_start == start)
        return;
    m_start= start;
    update();
    emit startChanged();
}

QPointF LinkItem::end() const
{
    return m_end;
}

void LinkItem::setEnd(const QPointF& end)
{
    if(end == m_end)
        return;
    m_end= end;
    emit endChanged();
}

PointList LinkItem::points() const
{
    return m_points;
}

void LinkItem::setPoints(const PointList& list)
{
    if(list == m_points)
        return;
    m_points= list;
    emit pointsChanged();
}

QRectF LinkItem::startBox() const
{
    return m_startBox;
}
QRectF LinkItem::endBox() const
{
    return m_endBox;
}
void LinkItem::setStartBox(QRectF rect)
{
    if(m_startBox == rect)
        return;
    m_startBox= rect;
    emit startBoxChanged();
}
void LinkItem::setEndBox(QRectF rect)
{
    if(rect == m_endBox)
        return;
    m_endBox= rect;
    emit endBoxChanged();
}

QSGNode* LinkItem::updatePaintNode(QSGNode* node, QQuickItem::UpdatePaintNodeData*)
{
    LinkNode* link= static_cast<LinkNode*>(node);
    if(!link)
    {
        link= new LinkNode();
        link->setColor(m_color);
    }
    link->update(m_start, m_end);
    return link;
}

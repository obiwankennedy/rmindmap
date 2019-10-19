#include "linknode.h"

#define PenWidth 4

#include <QLineF>

LinkNode::LinkNode() : m_geometry(QSGGeometry::defaultAttributes_Point2D(), 0)
{
    setGeometry(&m_geometry);
    m_geometry.setDrawingMode(GL_LINE_STRIP);
    m_geometry.allocate(6);
    setMaterial(&m_material);
}
void LinkNode::setColor(const QColor& color)
{
    m_material.setColor(color);
    markDirty(QSGNode::DirtyMaterial);
}
void LinkNode::update(QPointF const& p1, QPointF const& p2)
{
    qreal arrowLenght= 10.0;
    qreal arrowWidth= 8.0;
    qreal radius= 0.;
    qreal diameter= 0.;
    m_geometry.setLineWidth(PenWidth);
    QLineF line(p1, p2);
    auto pArrow= line.pointAt(1 - radius / line.length());
    auto startArrow= line.pointAt(1 - radius / line.length() - arrowLenght / line.length());

    QLineF arrowLine(startArrow, pArrow);
    QLineF arrowBase= arrowLine.normalVector();

    auto pointArrow= arrowBase.pointAt(arrowWidth / arrowBase.length());
    auto pointArrow2= arrowBase.pointAt(-arrowWidth / arrowBase.length());
    auto vertices= m_geometry.vertexDataAsPoint2D();
    {
        vertices[0].set(static_cast<float>(p1.x() + diameter), static_cast<float>(p1.y() + diameter));
        vertices[1].set(static_cast<float>(startArrow.x() + diameter), static_cast<float>(startArrow.y() + diameter));
        vertices[2].set(static_cast<float>(pointArrow.x() + diameter), static_cast<float>(pointArrow.y() + diameter));
        vertices[3].set(static_cast<float>(pArrow.x() + diameter), static_cast<float>(pArrow.y() + diameter));
        vertices[4].set(static_cast<float>(pointArrow2.x() + diameter), static_cast<float>(pointArrow2.y() + diameter));
        vertices[5].set(static_cast<float>(startArrow.x() + diameter), static_cast<float>(startArrow.y() + diameter));
    }
    markDirty(QSGNode::DirtyGeometry);
}

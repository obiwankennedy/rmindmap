#ifndef LINKNODE_H
#define LINKNODE_H

#include <QSGFlatColorMaterial>
#include <QSGGeometryNode>

class LinkNode : public QSGGeometryNode
{
public:
    LinkNode();
    void setColor(const QColor& color);
    void update(const QPointF& p1, const QPointF& p2);

private:
    QSGFlatColorMaterial m_material;
    QSGGeometry m_geometry;
};

#endif // LINKNODE_H

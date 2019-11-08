#include "mindnode.h"

#include <QFontMetricsF>
#include <QUuid>

#include "link.h"

MindNode::MindNode(QObject* parent) : QObject(parent), m_id(QUuid::createUuid().toString(QUuid::WithoutBraces))
{
    // comment
}
MindNode::~MindNode()= default;

void MindNode::setParentNode(MindNode* parent)
{
    m_parent= parent;
}

MindNode* MindNode::parentNode() const
{
    return m_parent;
}

void MindNode::setNextPosition(const QPointF& pos, Link* emiter)
{
    m_nextPositions.erase(emiter);
    m_nextPositions.insert(std::pair<Link*, QPointF>(emiter, pos));
    updatePosition();
}

QPointF MindNode::position() const
{
    return m_position;
}

void MindNode::setPosition(const QPointF& pos)
{
    if(m_position == pos)
        return;
    auto motion = pos-m_position;
    m_position= pos;
    emit positionChanged(m_position);
    if(isDragged())
        emit itemDragged(motion);
}

QVector2D MindNode::getVelocity() const
{
    return _velocity;
}

void MindNode::setVelocity(const QVector2D& velocity)
{
    _velocity= velocity;
}

QVector2D MindNode::getAcceleration() const
{
    return _acceleration;
}

void MindNode::setAcceleration(const QVector2D& acceleration)
{
    _acceleration= acceleration;
}

void MindNode::applyForce(const QVector2D& force)
{
    _acceleration+= force / m_mass;
}

int MindNode::getMass() const
{
    return m_mass;
}

QString MindNode::id() const
{
    return m_id;
}

void MindNode::setMass(int mass)
{
    m_mass= mass;
}

void MindNode::setText(QString text)
{
    if(m_text == text)
        return;

    m_text= text;
    emit textChanged(m_text);
}

qreal MindNode::contentWidth() const
{
    return m_width;
}

void MindNode::setContentWidth(qreal w)
{
    if(qFuzzyCompare(w, m_width))
        return;
    m_width= w;
    emit contentWidth();
}

qreal MindNode::contentHeight() const
{
    return m_height;
}

void MindNode::setContentHeight(qreal h)
{
    if(qFuzzyCompare(h, m_height))
        return;
    m_height= h;
    emit contentHeight();
}

QPointF MindNode::centerPoint() const
{
    return boundingRect().center();
}
QRectF MindNode::boundingRect() const
{
    QRectF boundingRect(m_position, QSizeF(m_width, m_height));
    return boundingRect;
}

void MindNode::updatePosition()
{
    if(m_nextPositions.size() == 0)
        return;

    if(m_nextPositions.size() == 1)
        setPosition(m_nextPositions.begin()->second);

    qreal x= 0;
    qreal y= 0;
    for(auto it= m_nextPositions.begin(); it != m_nextPositions.end(); ++it)
    {
        x+= it->second.x();
        y+= it->second.y();
    }
    x/= m_nextPositions.size();
    y/= m_nextPositions.size();

    setPosition(QPointF(x, y));
}

QString MindNode::text() const
{
    return m_text;
}

bool MindNode::open() const
{
    return m_open;
}
void MindNode::setOpen(bool op)
{
    if(op == m_open)
        return;
    m_open= op;
    emit openChanged();
    setLinkVisibility();
}

bool MindNode::isVisible() const
{
    return m_visible;
}
void MindNode::setVisible(bool op)
{
    if(op == m_visible)
        return;
    m_visible= op;
    emit visibleChanged();
    setLinkVisibility();
}

void MindNode::setLinkVisibility()
{
    bool visiblility= m_visible & m_open;
    std::for_each(m_subNodelinks.begin(), m_subNodelinks.end(), [visiblility](Link* link) {
        if(nullptr == link)
            return;
        link->setVisible(visiblility);
    });
}
void MindNode::addLink(Link* link)
{
    auto h= hasLink();
    m_subNodelinks.push_back(link);
    if(h != hasLink())
        emit hasLinkChanged();
}
bool MindNode::hasLink() const
{
    return !m_subNodelinks.empty();
}

QString MindNode::imageUri() const
{
    return m_imageUri;
}

void MindNode::setImageUri(const QString& uri)
{
    if(uri == m_imageUri)
        return;
    m_imageUri= uri;
    emit imageUriChanged();
}

void MindNode::setDragged(bool isdragged)
{
    if(m_isDragged == isdragged)
        return;
    m_isDragged= isdragged;
    emit isDraggedChanged();
}

void MindNode::setSelected(bool isSelected)
{
    if(m_selected == isSelected)
        return;
    m_selected= isSelected;
    emit selectedChanged();
}

void MindNode::setId(const QString& id)
{
    if(id == m_id)
        return;
    m_id= id;
    emit idChanged();
}

bool MindNode::isDragged() const
{
    return m_isDragged;
}

bool MindNode::selected() const
{
    return m_selected;
}

void MindNode::translate(const QPointF& motion)
{
    setPosition(m_position+motion);
}


#include "link.h"

#include "mindnode.h"
#include <QRectF>
#include <cmath>

Link::Link(QObject* parent) : QObject(parent)
{
    setText(tr("is linked"));
}

void Link::setDirection(const Direction& direction)
{
    m_dir= direction;
}

Link::Direction Link::direction() const
{
    return m_dir;
}

MindNode* Link::start() const
{
    return m_start;
}

void Link::setStart(MindNode* start)
{
    m_start= start;
    if(m_start)
    {
        m_start->addLink(this);
    }
    connect(m_start, &MindNode::positionChanged, this, &Link::linkChanged);
}

MindNode* Link::end() const
{
    return m_end;
}

QPointF Link::endPoint() const
{
    if(nullptr == m_end)
        return {};
    return m_end->centerPoint();
}

QPointF Link::startPoint() const
{
    if(nullptr == m_start)
        return {};
    return m_start->centerPoint();
}

void Link::setEnd(MindNode* end)
{
    m_end= end;
    connect(m_end, &MindNode::positionChanged, this, &Link::linkChanged);
}
void Link::computePosition()
{
    auto pos1= m_start->position();
    pos1.setY(pos1.y() + 50);
    m_end->setNextPosition(pos1, this);
}

void Link::setText(const QString& text)
{
    if(m_text == text)
        return;
    m_text= text;
    emit textChanged();
}

float Link::getStiffness() const
{
    return m_stiffness;
}

void Link::setStiffness(float stiffness)
{
    m_stiffness= stiffness;
}

float Link::getLength() const
{
    auto length= 100.;
    auto rect1= m_start->boundingRect();
    auto diag1= std::sqrt(std::pow(rect1.width(), 2) + std::pow(rect1.height(), 2)) / 2;

    auto rect2= m_end->boundingRect();
    auto diag2= std::sqrt(std::pow(rect2.width(), 2) + std::pow(rect2.height(), 2)) / 2;

    auto realDiag= std::max(diag1, diag2);

    return static_cast<float>(length + 2 * realDiag);
}

void Link::setVisible(bool vi)
{
    if(m_visible == vi)
        return;
    m_visible= vi;
    emit visibleChanged();

    if(nullptr != m_end)
        m_end->setVisible(vi);
}

bool Link::isVisible() const
{
    return m_visible;
}

QString Link::text() const
{
    return m_text;
}

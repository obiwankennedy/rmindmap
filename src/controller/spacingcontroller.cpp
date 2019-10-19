#include "spacingcontroller.h"

#include "data/link.h"
#include "data/mindnode.h"
#include "model/linkmodel.h"

#include <QLineF>
#include <chrono>
#include <cmath>
#include <thread>

const float k_attraction= 0.1f;
const float k_repulsion= 10000.f;

const float k_defaultDamping= 0.5f;
const float k_defaultSpringLength= 100.f;

bool isInside(QPointF pos1, QPointF pos2, float distance)
{
    auto d= pos1 - pos2;
    return (std::sqrt(d.x() * d.x() + d.y() * d.y()) < distance);
}

QPointF computeCenter(const MindNode* first, const std::vector<MindNode*>& data)
{
    QPointF pos= first->position();
    for(auto it= data.begin(); it != data.end(); ++it)
    {
        pos+= (*it)->position();
    }
    pos/= (data.size() + 1);
    return pos;
}

SpacingController::SpacingController(std::vector<MindNode*>& data, LinkModel* linkModel, QObject* parent)
    : QObject(parent), m_data(data), m_linkModel(linkModel)
{
}

void SpacingController::setRunning(bool run)
{
    if(run == m_running)
        return;
    m_running= run;
    emit runningChanged();
}

bool SpacingController::running() const
{
    return m_running;
}

void SpacingController::computeInLoop()
{
    while(m_running)
    {
        auto const allNodes= m_data;
        for(auto& node : allNodes)
        {
            applyCoulombsLaw(node, allNodes);
        }
        auto const allLinks= m_linkModel->getDataSet();
        for(auto& link : allLinks)
        {
            applyHookesLaw(link);
        }
        for(auto& node : allNodes)
        {
            node->setVelocity(node->getVelocity() * k_defaultDamping);
            node->setPosition(node->position() + node->getVelocity().toPointF());
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    Q_EMIT finished();
}

void SpacingController::applyCoulombsLaw(MindNode* node, std::vector<MindNode*> nodeList)
{
    auto globalRepulsionForce= QVector2D();
    for(auto const& otherNode : nodeList)
    {
        if(node == otherNode)
            continue;

        auto vect= QVector2D(node->position() - otherNode->position());
        auto length= vect.length();
        auto force= k_repulsion / std::pow(length, 2);

        globalRepulsionForce+= vect.normalized() * force;
    }

    node->setVelocity(node->getVelocity() + globalRepulsionForce);
}

void SpacingController::applyHookesLaw(Link* link)
{
    auto node1= link->start();
    auto node2= link->end();

    auto vect= QVector2D(node1->position() - node2->position());
    auto length= vect.length();
    auto force= k_attraction * std::max(length - k_defaultSpringLength, 0.f);

    node1->setVelocity(node1->getVelocity() + vect.normalized() * force * -1);
    node2->setVelocity(node2->getVelocity() + vect.normalized() * force);
}

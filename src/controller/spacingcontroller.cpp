/***************************************************************************
 *	Copyright (C) 2019 by Renaud Guezennec                                 *
 *   http://www.rolisteam.org/contact                                      *
 *                                                                         *
 *   This software is free software; you can redistribute it and/or modify *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "spacingcontroller.h"

#include "data/link.h"
#include "data/positioneditem.h"
#include "model/minditemmodel.h"

#include <QDebug>
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

QPointF computeCenter(const PositionedItem* first, const std::vector<PositionedItem*>& data)
{
    QPointF pos= first->position();
    for(auto it= data.begin(); it != data.end(); ++it)
    {
        pos+= (*it)->position();
    }
    pos/= (data.size() + 1);
    return pos;
}

SpacingController::SpacingController(MindItemModel* data, QObject* parent) : QObject(parent), m_model(data) {}

SpacingController::~SpacingController()= default;

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
        qDebug() << "running";
        auto const& allNodes= m_model->items(MindItem::NodeType);
        auto const& allPackage= m_model->items(MindItem::PackageType);

        std::vector<PositionedItem*> vec;
        vec.reserve(allNodes.size() + allPackage.size());
        std::transform(std::begin(allPackage), std::end(allPackage), std::back_inserter(vec),
                       [](MindItem* item) { return dynamic_cast<PositionedItem*>(item); });

        vec.erase(std::remove_if(std::begin(vec), std::end(vec), [](PositionedItem* item) { return nullptr == item; }),
                  std::end(vec));

        for(auto& node : vec)
        {
            applyCoulombsLaw(node, vec);
        }
        auto const allLinks= m_model->items(MindItem::LinkType);
        for(auto& item : allLinks)
        {
            auto link= dynamic_cast<Link*>(item);
            if(!link)
                continue;

            applyHookesLaw(link);
        }
        for(auto& node : vec)
        {
            node->setVelocity(node->getVelocity() * k_defaultDamping);
            if(!node->isDragged())
                node->setPosition(node->position() + node->getVelocity().toPointF());
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    Q_EMIT finished();
}

void SpacingController::applyCoulombsLaw(PositionedItem* node, std::vector<PositionedItem*> nodeList)
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

    if(node1 == nullptr || node2 == nullptr)
        return;

    auto vect= QVector2D(node1->position() - node2->position());
    auto length= vect.length();
    auto force= k_attraction * std::max(length - link->getLength(), 0.f);

    node1->setVelocity(node1->getVelocity() + vect.normalized() * force * -1);
    node2->setVelocity(node2->getVelocity() + vect.normalized() * force);
}

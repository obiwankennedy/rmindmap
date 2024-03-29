/***************************************************************************
 *	Copyright (C) 2022 by Renaud Guezennec                               *
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
#include "sidemenucontroller.h"
#include "controller/mindmapcontroller.h"
#include "model/minditemmodel.h"

FilteredModel::FilteredModel()
{
    setDynamicSortFilter(true);
}

void FilteredModel::setPattern(const QString& newPattern)
{
    if(m_pattern == newPattern)
        return;
    m_pattern= newPattern;
    emit patternChanged();
    invalidateFilter();
}

bool FilteredModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
    auto index= sourceModel()->index(source_row, 0, source_parent);

    auto type= index.data(MindItemModel::Type).value<MindItem::Type>();

    if(type != MindItem::NodeType)
        return false;

    if(m_criteria == NoCrit || m_pattern.isEmpty())
        return true;

    auto node= index.data(MindItemModel::Object).value<MindNode*>();
    auto parent= node->parentNode();

    bool res= false;
    switch(m_criteria)
    {
    case NoCrit:
        break;
    case NameCrit:
        res= node->text().contains(m_pattern, Qt::CaseInsensitive);
        break;
    case TagCrit:
        res= node->tagsText().contains(m_pattern, Qt::CaseInsensitive);
        break;
    case ParentOfCrit:
        res= parent->id().contains(m_pattern, Qt::CaseInsensitive);
        break;
    }

    return res;
}

FilteredModel::Criteria FilteredModel::criteria() const
{
    return m_criteria;
}

void FilteredModel::setCriteria(Criteria newCriteria)
{
    if(m_criteria == newCriteria)
        return;
    m_criteria= newCriteria;
    emit criteriaChanged();
    invalidateFilter();
}

const QString& FilteredModel::pattern() const
{
    return m_pattern;
}

SideMenuController::SideMenuController(QObject* parent) : QObject{parent}, m_model{new FilteredModel} {}

MindMapController* SideMenuController::controller() const
{
    return m_controller;
}

void SideMenuController::setController(MindMapController* newController)
{
    if(m_controller == newController)
        return;
    m_controller= newController;
    emit controllerChanged();
    m_model->setSourceModel(newController->itemModel());
}

FilteredModel* SideMenuController::model() const
{
    return m_model.get();
}

const QString& SideMenuController::pattern() const
{
    return m_pattern;
}

void SideMenuController::setPattern(const QString& newPattern)
{
    m_model->setPattern(newPattern);
}

FilteredModel::Criteria SideMenuController::criteria() const
{
    return m_model->criteria();
}

void SideMenuController::setCriteria(FilteredModel::Criteria newCriteria)
{
    m_model->setCriteria(newCriteria);
}

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
#ifndef SIDEMENUCONTROLLER_H
#define SIDEMENUCONTROLLER_H

#include <QObject>
#include <QPointer>
#include <QSortFilterProxyModel>
#include <memory>

class FilteredModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString pattern READ pattern WRITE setPattern NOTIFY patternChanged)
    Q_PROPERTY(Criteria criteria READ criteria WRITE setCriteria NOTIFY criteriaChanged)
public:
    enum Criteria
    {
        NoCrit,
        NameCrit,
        TagCrit,
        ParentOfCrit,
    };
    Q_ENUM(Criteria)

    FilteredModel();

    FilteredModel::Criteria criteria() const;
    void setCriteria(Criteria newCriteria);

    const QString& pattern() const;
    void setPattern(const QString& newPattern);

signals:
    void criteriaChanged();
    void patternChanged();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;

private:
    Criteria m_criteria= NoCrit;
    QString m_pattern;
};

class MindMapController;
class SideMenuController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(MindMapController* controller READ controller WRITE setController NOTIFY controllerChanged)
    Q_PROPERTY(FilteredModel* model READ model CONSTANT)
    Q_PROPERTY(FilteredModel::Criteria criteria READ criteria WRITE setCriteria NOTIFY criteriaChanged)
    Q_PROPERTY(QString pattern READ pattern WRITE setPattern NOTIFY patternChanged)

public:
    explicit SideMenuController(QObject* parent= nullptr);

    MindMapController* controller() const;
    void setController(MindMapController* newController);

    FilteredModel* model() const;

    const QString& pattern() const;
    void setPattern(const QString& newPattern);

    FilteredModel::Criteria criteria() const;
    void setCriteria(FilteredModel::Criteria newCriteria);

signals:
    void controllerChanged();
    void patternChanged();
    void criteriaChanged();

private:
    QPointer<MindMapController> m_controller;
    std::unique_ptr<FilteredModel> m_model;
    QString m_pattern;
};

#endif // SIDEMENUCONTROLLER_H

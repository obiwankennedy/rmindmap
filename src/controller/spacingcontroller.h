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
#ifndef SPACINGCONTROLLER_H
#define SPACINGCONTROLLER_H

#include <QObject>
#include <QPointer>

class MindNode;
class MindItemModel;
class Link;
class PositionedItem;
class SpacingController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
public:
    explicit SpacingController(MindItemModel* data, QObject* parent= nullptr);
    ~SpacingController();

    bool running() const;
signals:
    void finished();
    void runningChanged();

public slots:
    void computeInLoop();
    void setRunning(bool run);

private:
    void applyCoulombsLaw(PositionedItem* node, std::vector<PositionedItem*> nodeList);
    void applyHookesLaw(Link* link);
    void attractToCenter();

private:
    QPointer<MindItemModel> m_model;
    bool m_running= true;
};

#endif // SPACINGCONTROLLER_H

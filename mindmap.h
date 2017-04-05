/***************************************************************************
    *   Copyright (C) 2016 by Renaud Guezennec                                *
    *   http://www.rolisteam.org/contact                                      *
    *                                                                         *
    *   rolisteam is free software; you can redistribute it and/or modify     *
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

#ifndef MINDMAP_H
#define MINDMAP_H

#include <QGraphicsScene>
#include "items/node.h"
#include "stringmanager/stringmanager.h"
#include "preferences/preferencesmanager.h"
#include "items/packageitem.h"

class MindMap : public QGraphicsScene
{
public:
    MindMap(QObject* parent);

    void readFromData(QJsonObject &root);
    void writeToData(QJsonObject &root);
    void clearRoots();
    StringManager* getStringManager() const;
    void setStringManager(StringManager *stringManager);

    void addNodeAt(QPoint pos);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

private:
    QList<Node*> m_roots;
    StringManager* m_stringManager;
    PreferencesManager* m_preferences;
    PackageItem* m_currentPackage;
};

#endif // MINDMAP_H

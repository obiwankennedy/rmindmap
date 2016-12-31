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
#include <QTextStream>
#include <QList>

#include "preferences/preferencesmanager.h"
#include "items/node.h"
#include "items/edge.h"
#include "serializable.h"

/**
 * @brief The MindMap class
 */
class MindMap : public QGraphicsScene, public Serialisable
{
public:
    MindMap(QObject* parent);

    void readFromText(QTextStream& in);


    void appendRoot(Node* root);

    int getDepth(QString line);
    Node *addNodeAt(QPoint pos);

    StringManager *getStringManager() const;
    void setStringManager(StringManager *stringManager);

    void linkItem(Node *first, Node *second);

    virtual void readFromData(QJsonObject&);
    virtual void writeToData(QJsonObject&);
private:
    QList<Node*> m_roots;
    PreferencesManager* m_preferences;
    StringManager* m_stringManager;

};

#endif // MINDMAP_H

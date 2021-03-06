/***************************************************************************
 *	Copyright (C) 2011 by Renaud Guezennec                             *
 *   http://renaudguezennec.homelinux.org/accueil,3.html                   *
 *                                                                         *
 *   rmindmap is free software; you can redistribute it and/or modify  *
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
#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <QSettings>
#include <QDataStream>
#include <QJsonObject>

class SettingsUser
{
public:
    virtual void readSettings(QSettings&) = 0;
    virtual void writeSettings(QSettings&) = 0;
};
class EdgableItems;
class GenericMindMapItem;
class QGraphicsScene;

class Serialisable
{
public:
    virtual void readFromData(QJsonObject&) = 0;
    virtual void writeToData(QJsonObject&) = 0;
 //  virtual void readFromData(QJsonObject&,EdgableItems* parent,QGraphicsScene* m_scene) = 0;
 //   virtual void writeToData(QJsonObject&,EdgableItems* parent,QHash<QString,GenericMindMapItem*>* done) = 0;
};

#endif // SERIALIZABLE_H

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
#ifndef STRINGMANAGER_H
#define STRINGMANAGER_H

#include <QObject>
#include <QList>
#include <QMap>

#include "src/serializable.h"
/**
 * @brief The StringManager class - provides features to translate a mindmap dynamically.
 */
class StringManager : public QObject, public Serialisable
{
    Q_OBJECT
public:
    explicit StringManager(QObject *parent = 0);

    void setValue(QString key, QString value);

    QString getValue(QString key);

    void setCurrentLang(int);

    virtual void readFromData(QJsonObject&,EdgableItems*,QGraphicsScene* scene);
    virtual void writeToData(QJsonObject&,EdgableItems*,QHash<QString,GenericMindMapItem*>* done);


private:
    QMap<QString,QString> m_currentLang;
    QList<QMap<QString,QString> > m_availableLang;


    
};

#endif // STRINGMANAGER_H

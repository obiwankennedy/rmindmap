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
#include "stringmanager.h"
#include <QDebug>


StringManager::StringManager(QObject *parent) :
    QObject(parent)
{

}


void StringManager::readFromData(QJsonObject&,EdgableItems*)
{
    /*settings.beginGroup("StringManager");
    m_availableLang = settings.value("Availablelang");
    m_currentLang = settings.value("currentlang");*/


    /*out >> m_availableLang;
    QMap<QString,QString> lang;
    out >> lang;
    m_currentLang.unite(lang);*/

   // qDebug()<< m_currentLang.size() << m_availableLang.size();

}

void StringManager::writeToData(QJsonObject&,EdgableItems*)
{
    /*settings.beginGroup("StringManager");
    settings.setValue("Availablelang",m_availableLang);
    settings.setValue("currentlang",m_currentLang);*/
   /* in << m_availableLang;
    in << m_currentLang;*/

}
void StringManager::setValue(QString key, QString value)
{
    m_currentLang.insert(key,value);
}

QString StringManager::getValue(QString key)
{
    return m_currentLang.value(key);
}

void StringManager::setCurrentLang(int i)
{
    if((i>=0)&&(i<m_availableLang.size()))
    {
        m_currentLang = m_availableLang[i];
    }
}

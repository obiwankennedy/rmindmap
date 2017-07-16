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
#include <QJsonArray>


StringManager::StringManager(QObject *parent) :
    QObject(parent)
{

}


void StringManager::readFromData(QJsonObject&,EdgableItems*,QGraphicsScene* scene)
{
    /*settings.beginGroup("StringManager");
    m_availableLang = settings.value("Availablelang");
    m_currentLang = settings.value("currentlang");*/
    /*out >> m_availableLang;
    QMap<QString,QString> lang;
    out >> lang;
    m_currentLang.unite(lang);*/

   // qDebug()<< m_currentLang.size() << m_availableLang.size();

    QJsonObject stringManager = out["stringManager"].toObject();

    QJsonArray array = stringManager["curretLang"].toArray();
    for(auto it = array.begin(); it != array.end();++it)
    {
        QJsonObject duo = it->toObject();
        QString key = duo["key"].toString();
        QString val = duo["value"].toString();
        m_currentLang.insert(key,val);
    }

    QJsonArray array2 = stringManager["availLang"].toArray();
    for(auto it = array2.begin(); it != array2.end();++it)
    {
        QJsonArray array3 = it->toArray();
        QMap<QString, QString> map;
        for(auto it = array3.begin(); it != array3.end();++it)
        {
            QJsonObject duo = it->toObject();
            QString key = duo["key"].toString();
            QString val = duo["value"].toString();

            map.insert(key,val);
        }
        m_availableLang.append(map);

    }
}

void StringManager::writeToData(QJsonObject&,EdgableItems*,QHash<QString,GenericMindMapItem*>* done)
{
    /*settings.beginGroup("StringManager");
    settings.setValue("Availablelang",m_availableLang);
    settings.setValue("currentlang",m_currentLang);*/
  /*  in << m_availableLang;
    in << m_currentLang;*/

    QJsonObject stringManager;

    QJsonArray array;
    //stringManager["currentLangSize"]=m_currentLang.size();

    for(auto key : m_currentLang.keys())
    {
        QJsonObject duo;
        duo["key"]=key;
        duo["value"]=m_currentLang[key];
        array.append(duo);
    }
    stringManager["curretLang"]=array;

    QJsonArray array3;
    for(auto lang : m_availableLang)
    {
        QJsonArray array2;
        for(auto key : lang.keys())
        {
            QJsonObject duo;
            duo["key"]=key;
            duo["value"]=m_currentLang[key];
            array2.append(duo);
        }
        array3.append(array2);
    }
    stringManager["availLang"]=array3;

    in["stringManager"]=stringManager;

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

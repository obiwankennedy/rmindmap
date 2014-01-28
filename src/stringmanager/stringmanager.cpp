#include "stringmanager.h"
#include <QDebug>


StringManager::StringManager(QObject *parent) :
    QObject(parent)
{

}


void StringManager::readFromData(QDataStream& out)
{
    /*settings.beginGroup("StringManager");
    m_availableLang = settings.value("Availablelang");
    m_currentLang = settings.value("currentlang");*/
    out >> m_availableLang;
    QMap<QString,QString> lang;
    out >> lang;
    m_currentLang.unite(lang);

   // qDebug()<< m_currentLang.size() << m_availableLang.size();

}

void StringManager::writeToData(QDataStream& in)
{
    /*settings.beginGroup("StringManager");
    settings.setValue("Availablelang",m_availableLang);
    settings.setValue("currentlang",m_currentLang);*/
    in << m_availableLang;
    in << m_currentLang;

}
void StringManager::setValue(QString key, QString value)
{
   // qDebug()<< key << value;
    m_currentLang.insert(key,value);
}

QString StringManager::getValue(QString key)
{
//    qDebug()<< "Asked key" << key;
    return m_currentLang.value(key);
}

void StringManager::setCurrentLang(int i)
{
    if((i>=0)&&(i<m_availableLang.size()))
    {
        m_currentLang = m_availableLang[i];
    }
}

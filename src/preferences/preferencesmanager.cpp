/***************************************************************************
    *	Copyright (C) 2010 by Renaud Guezennec                             *
    *   http://renaudguezennec.homelinux.org/accueil,3.html                   *
    *                                                                         *
    *   rolisteam is free software; you can redistribute it and/or modify  *
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
#include "preferencesmanager.h"
#include <QSettings>
#include <QDir>
#include <QDebug>


PreferencesManager::PreferencesManager()
    : m_optionDictionary(NULL)
{
    m_optionDictionary = new QMap<QString,QVariant>;
    
    ColorTheme* theme = new ColorTheme();
    QColor color1;
    QColor color0;

    m_defaultTheme = theme->getId();


    color0.setHsv(0,20,120);
    color0.setAlpha(20);
    color1.setHsv(0,120,255);
    color1.setAlpha(20);
    theme->setColorBg0(color0);
    theme->setColorBg0(color1);
    theme->setTextColor(QColor(Qt::black));
    m_colorThemeNodeModel.insert(theme->getId(),theme);

    //Default value
    for(int i=0;i<255;i+=5)
    {
        ColorTheme* theme = new ColorTheme();
        QColor color1;
        QColor color0;

        color1.setHsv(i,255,255);
        color0.setHsv(i,255,255);

        theme->setColorBg0(color0);
        theme->setColorBg0(color1);
        int a = color0.green()+color0.blue()+color0.red()-381;
        if(a>0)
        {
            theme->setTextColor(QColor(Qt::black));
        }
        else
        {
            theme->setTextColor(QColor(Qt::white));
        }
        m_colorThemeNodeModel.insert(theme->getId(),theme);
    }
    
}



PreferencesManager::~PreferencesManager()
{
    delete m_optionDictionary;
}

PreferencesManager* PreferencesManager::m_singleton = NULL;

ColorTheme* PreferencesManager::getDefaultNodeColorTheme()
{
    return m_colorThemeNodeModel.value(m_defaultTheme);
}

int PreferencesManager::getColorThemeCount()
{
    return m_colorThemeNodeModel.size();
}

ColorTheme* PreferencesManager::getColorThemeById(int i)
{
    if(i>=0 && i<getColorThemeCount())
    {
        return m_colorThemeNodeModel.at(i);
    }
    else
        return nullptr;
}

int PreferencesManager::indexOf(ColorTheme* theme)
{
   return m_colorThemeNodeModel.indexOf(theme);
}
PreferencesManager* PreferencesManager::getInstance()
{
    
    if(m_singleton == NULL)
        m_singleton = new PreferencesManager;
    
    return m_singleton;
}
bool PreferencesManager::registerValue(QString key,QVariant value, bool overwrite)
{
    if((overwrite)||(!m_optionDictionary->contains(key)))
    {
        m_optionDictionary->insert(key,value);
        return true;
    }
    else
        return false;
}
const QVariant PreferencesManager::value(QString key,QVariant defaultValue)
{
    if(m_optionDictionary->contains(key))
        return m_optionDictionary->value(key);
    else
        return defaultValue;
    
}

void PreferencesManager::readSettings(QSettings & settings)
{
    settings.beginGroup("preferences");
    QVariant variant = settings.value("map",*m_optionDictionary);
    if(variant.canConvert<QMap<QString,QVariant> >())
    {

        *m_optionDictionary = variant.value<QMap<QString,QVariant> >();
    }
    

    for(int i = 0;m_optionDictionary->size()>i;i++)
    {
        // qDebug() << "key " << m_optionDictionary->keys().at(i) << "value: " << m_optionDictionary->value(m_optionDictionary->keys().at(i),true).toString();


    }
    
    settings.endGroup();
    
}
void PreferencesManager::writeSettings(QSettings & settings)
{
    settings.beginGroup("preferences");
    settings.setValue("map",*m_optionDictionary);
    settings.endGroup();

    for(int i = 0;m_optionDictionary->size()>i;i++)
    {
        // qDebug() << "key " << m_optionDictionary->keys().at(i) << "value: " << m_optionDictionary->value(m_optionDictionary->keys().at(i),true).toString();


    }
}
ColorTheme* PreferencesManager::getNodeColorTheme(QString id)
{
    if(m_colorThemeNodeModel.contains(id))
    {
        return m_colorThemeNodeModel.value(id);
    }
    else
    {
        return getDefaultNodeColorTheme();
    }
}

ColorTheme* PreferencesManager::getEdgeColorTheme(QString id)
{
    if(m_colorThemeEdgeModel.contains(id))
    {
        return m_colorThemeEdgeModel.value(id);
    }
    else
    {
        return getDefaultNodeColorTheme();
    }
}

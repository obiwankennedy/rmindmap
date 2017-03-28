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
#include "genericmindmapitem.h"

#include <QDebug>
#include <QLinearGradient>
#include <QUuid>
/////////////////////////////////////////
//
//      Code of ColorTheme
//
/////////////////////////////////////////
ColorTheme::ColorTheme()
    : m_lineStyle(Qt::SolidLine),m_id(QUuid::createUuid().toString())
{

}

QBrush ColorTheme::getBursh(qreal x1, qreal y1, qreal x2, qreal y2)
{
    QLinearGradient linear(x1,y1,x2,y2);
    linear.setColorAt(0,m_colorBg0);
    linear.setColorAt(1,m_colorBg1);
    return QBrush(linear);
}
QColor ColorTheme::getTextColor() const
{
    return m_textColor;
}
void ColorTheme::setTextColor(const QColor &textColor)
{
    m_textColor = textColor;
}

QColor ColorTheme::getColorBg0() const
{
    return m_colorBg0;
}
void ColorTheme::setColorBg0(const QColor &colorBg0)
{
    m_colorBg0 = colorBg0;
}

QColor ColorTheme::getColorBg1() const
{
    return m_colorBg1;
}
void ColorTheme::setColorBg1(const QColor &colorBg1)
{
    m_colorBg1 = colorBg1;
}

Qt::PenStyle ColorTheme::getLineStyle() const
{
    return m_lineStyle;
}

void ColorTheme::setLineStyle(const Qt::PenStyle &lineStyle)
{
    m_lineStyle = lineStyle;
}

QString ColorTheme::getId() const
{
    return m_id;
}

void ColorTheme::setId(const QString &id)
{
    m_id = id;
}





/////////////////////////////////////////
//
//      Code of GenericMindMapItem
//
/////////////////////////////////////////
GenericMindMapItem::GenericMindMapItem()
    : m_stringManager(NULL),m_colorTheme(NULL),m_id(QUuid::createUuid().toString())
{
    setAcceptDrops(true);
    setAcceptHoverEvents(true);

}

void GenericMindMapItem::setStringManager(StringManager* stringManager)

{
    m_stringManager = stringManager;
}

void GenericMindMapItem::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
    //qDebug() << getText() << "GenericMindMapItem::hoverEnterEvent";
}

void GenericMindMapItem::dragEnterEvent ( QGraphicsSceneDragDropEvent * event )
{
    //qDebug() << getText() << "GenericMindMapItem::dragEnterEvent";
}

ColorTheme *GenericMindMapItem::getColorTheme() const
{
    return m_colorTheme;
}

void GenericMindMapItem::setColorTheme(ColorTheme *colorTheme)
{
    m_colorTheme = colorTheme;
}

QString GenericMindMapItem::getId() const
{
    return m_id;
}

void GenericMindMapItem::setId(const QString &id)
{
    m_id = id;
}
void  GenericMindMapItem::setGeometry(int w,int h)
{
    qDebug() << w << h;
}

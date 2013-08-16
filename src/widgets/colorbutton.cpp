/***************************************************************************
 *	Copyright (C) 2011 by Renaud Guezennec                             *
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
#include "colorbutton.h"

#include <QColorDialog>
#include <QStyleFactory>

#include <QPaintEvent>
#include <QStyleOptionFocusRect>
#include <QDebug>

ColorButton::ColorButton(QWidget * parent,const QColor & color)
    : QPushButton(parent), m_color(color), m_dialog(color, this)
{
    setStyle(QStyleFactory::create("fusion"));

    setColor(m_color);

    setMaximumSize(sizeHint());

    // should never be the default button
    setDefault(false);
    setAutoDefault(false);

    connect(this, SIGNAL(pressed()), &m_dialog, SLOT(open()));
    connect(&m_dialog, SIGNAL(colorSelected(const QColor &)), this, SLOT(setColor(const QColor &)));
}


const QColor& ColorButton::color() const
{
    return m_color;
}

QSize ColorButton::sizeHint() const
{
    return QSize(28,20);
}

void ColorButton::setColor(const QColor & color)
{
    m_color = color;
    QPalette tmp = palette();
    tmp.setColor(QPalette::Button,m_color);
    tmp.setColor(QPalette::Window,m_color);
    setPalette(tmp);
    setAutoFillBackground(true);
    setStyleSheet(QString("ColorButton { background-color: rgb(%1,%2,%3);}").arg(color.red()).arg(color.green()).arg(color.blue()));
    emit colorChanged(m_color);
}



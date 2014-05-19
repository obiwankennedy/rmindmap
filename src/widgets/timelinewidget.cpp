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
#include <QPainter>
#include <QRect>
#include <QSize>
#include <QPalette>

#include "timelinewidget.h"

TimeLineWidget::TimeLineWidget()
    : m_divisionCount(100),m_currentPosition(0)
{
    setMinimumSize(200,20);
}
void TimeLineWidget::paintEvent( QPaintEvent * event)
{
    QPainter painter(this);

    painter.setBrush(palette().color(QPalette::AlternateBase));
    painter.drawRoundedRect(0,0,size().width(),size().height(),10.0,10.0);
    painter.setBrush(Qt::NoBrush);
    //painter.fillRect(0,0,size().width(),size().height(),);

    int step=(size().width()-10)/m_divisionCount;
    int height=geometry().height();
    //int widget=rect().width();
    for(quint32 i = 0;i<m_divisionCount;++i)
    {
        painter.drawLine(10+i*step,height,10+i*step,height/2);
    }


    if(m_currentPosition>m_totalTime)
    {
        m_currentPosition=m_totalTime;
    }
    qreal pos = m_currentPosition*(size().width()-10)/m_totalTime;

    painter.setPen(Qt::red);
    painter.drawLine(pos+10,height,pos+10,height/2);

}
void TimeLineWidget::setIntervalTime(qreal a)
{
    m_intervalTime = a;
    m_totalTime = m_intervalTime*m_divisionCount;
}

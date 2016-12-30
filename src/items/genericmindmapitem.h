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
#ifndef GENERICMINDMAPITEM_H
#define GENERICMINDMAPITEM_H

#include <QGraphicsItem>

#include "stringmanager/stringmanager.h"

#include "src/serializable.h"

class ColorTheme
{
public:
    ColorTheme();

    QBrush getBursh(qreal x1, qreal y1, qreal x2, qreal y2);

    QColor getTextColor() const;
    void setTextColor(const QColor &textColor);

    QColor getColorBg0() const;
    void setColorBg0(const QColor &colorBg0);

    QColor getColorBg1() const;
    void setColorBg1(const QColor &colorBg1);

    Qt::PenStyle getLineStyle() const;
    void setLineStyle(const Qt::PenStyle &lineStyle);

private:
    QColor m_textColor;
    QColor m_colorBg0;
    QColor m_colorBg1;
    Qt::PenStyle m_lineStyle;
};

class GenericMindMapItem : public QGraphicsObject,public Serialisable
{
public:
    GenericMindMapItem();
   virtual QString getName() const = 0;
   virtual QString getText() const = 0;

   virtual QPixmap getIcon() const =0;



    virtual void setStringManager(StringManager* stringManager);

    virtual void setGeometry(int w,int h);

    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    virtual void dragEnterEvent ( QGraphicsSceneDragDropEvent * event );

    ColorTheme* getColorTheme() const;
    void setColorTheme(ColorTheme* colorTheme);

public slots:
    virtual void setText(QString) = 0;
    virtual void setName(QString&) = 0;

protected:
    StringManager* m_stringManager;
    ColorTheme *m_colorTheme;

};

class Edge;
class EdgableItems : public GenericMindMapItem
{
public:
    virtual void addEdge(Edge* ) = 0;

    virtual QString getUuid() = 0;
    virtual QPointF middlePoint() = 0;
};

#endif // GENERICMINDMAPITEM_H

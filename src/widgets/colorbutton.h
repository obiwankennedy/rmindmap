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
#ifndef COLORLABEL_H
#define COLORLABEL_H

#include <QPushButton>
#include <QColorDialog>
/**
  * @brief shows selected color and detect any event such as click or doubleclick
  */
class ColorButton : public QPushButton
{
    Q_OBJECT
public:
    /**
      * @brief default constructor
      */
    ColorButton(QWidget * parent = NULL,const QColor & color = QColor("tan"));

   /**
     * @brief accessor to the color.
     */
   const QColor& color() const;

   virtual QSize sizeHint() const;
signals:
   /**
     * @brief send off the current color
     */
    void colorChanged(const QColor& p);

public slots:
    /**
      * @brief defines the color of the colorbutton
      */
   void setColor(const QColor & color);


private:
    QColor m_color;
    QColorDialog m_dialog;
};

#endif // COLORLABEL_H

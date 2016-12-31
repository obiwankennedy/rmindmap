/***************************************************************************
    *   Copyright (C) 2016 by Renaud Guezennec                                *
    *   http://www.rolisteam.org/contact                                      *
    *                                                                         *
    *   rolisteam is free software; you can redistribute it and/or modify     *
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

#ifndef MENUSTYLEITEM_H
#define MENUSTYLEITEM_H

#include <QDialog>
#include "preferences/preferencesmanager.h"
#include "items/genericmindmapitem.h"

namespace Ui {
class MenuStyleItem;
}

class MenuStyleItem : public QDialog
{
    Q_OBJECT

public:
    explicit MenuStyleItem(QWidget *parent = 0);
    ~MenuStyleItem();

    void initActions();

    void setItem(EdgableItem* item);
    EdgableItem* getItem()const;

private:
    Ui::MenuStyleItem *ui;

    QAction* m_menuAct;
    QAction* m_fontAct;
    QAction* m_patelleAct;
    QAction* m_hierarchyAct;
    QAction* m_shapeAct;
    QAction* m_gatheringAct;

    PreferencesManager* m_preferences;
    EdgableItem* m_element;
};

#endif // MENUSTYLEITEM_H

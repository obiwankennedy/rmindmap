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

#include "menustyleitem.h"
#include "ui_menustyleitem.h"
#include <QDebug>
#include <QMouseEvent>
#include <QToolButton>

MenuStyleItem::MenuStyleItem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MenuStyleItem)
{
    ui->setupUi(this);
    m_preferences = PreferencesManager::getInstance();
    initActions();

    setWindowFlags(Qt::FramelessWindowHint|Qt::BypassWindowManagerHint|Qt::Popup);
    setMouseTracking(true);

}

MenuStyleItem::~MenuStyleItem()
{
    delete ui;
}

void MenuStyleItem::initActions()
{
    m_menuAct = new QAction(tr("Menu"),this);
    m_menuAct->setIcon(QIcon(":/icons/resources/icons/menu.png"));
    ui->m_menu->setDefaultAction(m_menuAct);
    connect(m_menuAct,&QAction::triggered,[=](){
       ui->m_pageWidget->setCurrentIndex(0);
    });

    m_fontAct = new QAction(tr("Font"),this);
    m_fontAct->setIcon(QIcon(":/icons/resources/icons/font.png"));
    ui->m_font->setDefaultAction(m_fontAct);
    connect(m_fontAct,&QAction::triggered,[=](){
       ui->m_pageWidget->setCurrentIndex(1);
    });

    m_patelleAct= new QAction(tr("Palette"),this);
    m_patelleAct->setIcon(QIcon(":/icons/resources/icons/palette.png"));
    ui->m_colorTheme->setDefaultAction(m_patelleAct);
    connect(m_patelleAct,&QAction::triggered,[=](){
       ui->m_pageWidget->setCurrentIndex(2);
    });

    m_hierarchyAct= new QAction(tr("Hierarchi"),this);
    m_hierarchyAct->setIcon(QIcon(":/icons/resources/icons/hierarchy.png"));
    ui->m_hierarchyType->setDefaultAction(m_hierarchyAct);
    connect(m_hierarchyAct,&QAction::triggered,[=](){
       ui->m_pageWidget->setCurrentIndex(3);
    });

    m_shapeAct= new QAction(tr("Shape"),this);
    m_shapeAct->setIcon(QIcon(":/icons/resources/icons/shape.png"));
    ui->m_shape->setDefaultAction(m_shapeAct);
    connect(m_shapeAct,&QAction::triggered,[=](){
       ui->m_pageWidget->setCurrentIndex(4);
    });

    m_gatheringAct= new QAction(tr("Gathering"),this);
    m_gatheringAct->setIcon(QIcon(":/icons/resources/icons/gathering.png"));
    ui->m_gatheringChild->setDefaultAction(m_gatheringAct);
    connect(m_gatheringAct,&QAction::triggered,[=](){
       ui->m_pageWidget->setCurrentIndex(5);
    });

    // init the palette menu
    for(int i = 0; i<m_preferences->getColorThemeCount();++i)
    {
        ColorTheme* item = m_preferences->getColorThemeById(i);

        QPixmap img = item->getPixmap();

        QToolButton* label = new QToolButton();
        label->setIconSize(img.size());
        QAction* act = new QAction(this);
        act->setData(i);
        connect(act,&QAction::triggered,[=](){
           // QAction* act = qobject_cast<QAction*>(sender());
            qDebug() << "tset" << act;
            if(nullptr!= act)
            {
                m_element->setColorTheme(m_preferences->getColorThemeById(act->data().toInt()));
            }
        });
        act->setIcon(QIcon(img));
        label->setDefaultAction(act);
        //label->setPixmap(img);
        int row = i/3;
        int column = i%3;

        ui->m_gridOfStyle->addWidget(label,row,column);
    }

}

void MenuStyleItem::setItem(EdgableItem* item)
{
    m_element = item;
}

EdgableItem* MenuStyleItem::getItem() const
{
    return m_element;
}

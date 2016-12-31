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
#include "detailpanel.h"
#include "ui_detailpanel.h"

DetailPanel::DetailPanel(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::DetailPanel),m_alwaysVisible(true)
{
    m_currentEdge = NULL;
    m_currentNode = NULL;
    m_currentPackage = NULL;
    ui->setupUi(this);

    /// Generic properties
    connect(ui->m_textEdit,SIGNAL(textEdited(QString)),this,SLOT(updateGenericItem()));
    connect(ui->m_xEdit,SIGNAL(valueChanged(int)),this,SLOT(updateGenericItem()));
    connect(ui->m_yEdit,SIGNAL(valueChanged(int)),this,SLOT(updateGenericItem()));
    connect(ui->m_heightEdit,SIGNAL(valueChanged(int)),this,SLOT(updateGenericItem()));
    connect(ui->m_widthEdit,SIGNAL(valueChanged(int)),this,SLOT(updateGenericItem()));

    connect(ui->m_nodeDescriptionEdit,SIGNAL(textChanged()),this,SLOT(updateCurrentNode()));
    connect(ui->m_bgColorChooser,SIGNAL(colorChanged(QColor)),this,SLOT(updateCurrentNode()));
    connect(ui->m_borderColorChooser,SIGNAL(colorChanged(QColor)),this,SLOT(updateCurrentNode()));
    connect(ui->m_edgeArrowType,SIGNAL(currentIndexChanged(int)),this,SLOT(updateCurrentEdge()));
    connect(ui->m_packageLine,SIGNAL(currentIndexChanged(int)),this,SLOT(updateCurrentPackage()));
}

DetailPanel::~DetailPanel()
{
    delete ui;
}
void DetailPanel::setCurrentNode(Node* node)
{

    m_currentEdge = NULL;
    m_currentNode = NULL;
    m_currentPackage = NULL;
    ui->m_stackedWidget->setCurrentIndex(0);
    ui->m_nodeDescriptionEdit->setText(node->description());
    ui->m_textEdit->setText(node->getText());
 //   ui->m_bgColorChooser->setColor(node->bgColor());
 //   ui->m_borderColorChooser->setColor(node->color());

    m_currentNode=node;
}

void DetailPanel::setCurrentEdge(Edge* edge)
{

    m_currentEdge = NULL;
    m_currentNode = NULL;
    m_currentPackage = NULL;
    ui->m_stackedWidget->setCurrentIndex(1);
    ui->m_textEdit->setText(edge->getText());
    ui->m_edgeArrowType->setCurrentIndex((int)edge->getKind());
    m_currentEdge = edge;
}



void DetailPanel::updateCurrentNode()
{
    if(NULL!=m_currentNode)
    {
        m_currentNode->setDescription(ui->m_nodeDescriptionEdit->document()->toHtml());
      //  m_currentNode->setColor(ui->m_borderColorChooser->color());
       // m_currentNode->setBgColor(ui->m_bgColorChooser->color());
    }

}

void DetailPanel::updateCurrentEdge()
{
    if(NULL!=m_currentEdge)
    {
        m_currentEdge->setKind((Edge::ArrowHead)ui->m_edgeArrowType->currentIndex());
    }
}

void DetailPanel::updateCurrentPackage()
{
    if(NULL!=m_currentPackage)
    {
        m_currentPackage->setBorder((PackageItem::Border)ui->m_packageLine->currentIndex());
        m_currentPackage->setPos(ui->m_xEdit->value(),ui->m_yEdit->value());
    }
}
void DetailPanel::updateGenericItem()
{
    if(NULL!=m_currentGenericItem)
    {
        m_currentGenericItem->setText(ui->m_textEdit->text());
        m_currentGenericItem->setPos(ui->m_xEdit->value(),ui->m_yEdit->value());
        m_currentGenericItem->setGeometry(ui->m_widthEdit->value(),ui->m_heightEdit->value());
    }
}
void DetailPanel::setCurrentPackage(PackageItem* p)
{
    m_currentEdge = NULL;
    m_currentNode = NULL;
    m_currentPackage = NULL;
    ui->m_stackedWidget->setCurrentIndex(2);
    ui->m_textEdit->setText(p->getText());


    m_currentPackage = p;
}

void DetailPanel::setCurrentGenericItem(GenericMindMapItem* p)
{
    m_currentEdge = NULL;
    m_currentNode = NULL;
    m_currentPackage = NULL;
    m_currentGenericItem = NULL;
    ui->m_xEdit->setValue(p->pos().x());
    ui->m_yEdit->setValue(p->pos().y());
    ui->m_widthEdit->setValue(p->boundingRect().width());
    ui->m_heightEdit->setValue(p->boundingRect().height());
    m_currentGenericItem = p;
    if(NULL!=dynamic_cast<PackageItem*>(p))
    {
        ui->m_xEdit->setEnabled(true);
        ui->m_yEdit->setEnabled(true);
        ui->m_widthEdit->setEnabled(true);
        ui->m_heightEdit->setEnabled(true);
        setCurrentPackage(dynamic_cast<PackageItem*>(p));
    }
    else if(NULL!=dynamic_cast<Node*>(p))
    {
        ui->m_xEdit->setEnabled(true);
        ui->m_yEdit->setEnabled(true);
        ui->m_widthEdit->setEnabled(false);
        ui->m_heightEdit->setEnabled(false);
        setCurrentNode(dynamic_cast<Node*>(p));
    }
    else if(NULL!=dynamic_cast<Edge*>(p))
    {
        ui->m_xEdit->setEnabled(false);
        ui->m_yEdit->setEnabled(false);
        ui->m_widthEdit->setEnabled(false);
        ui->m_heightEdit->setEnabled(false);
        setCurrentEdge(dynamic_cast<Edge*>(p));
    }

}
void DetailPanel::setAlwaysVisible(bool b)
{
    m_alwaysVisible = b;
}

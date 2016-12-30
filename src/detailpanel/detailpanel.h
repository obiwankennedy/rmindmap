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
#ifndef DETAILPANEL_H
#define DETAILPANEL_H

#include <QDockWidget>


#include "items/node.h"
#include "items/edge.h"
#include "items/packageitem.h"

#include "colorbutton.h"

namespace Ui {
class DetailPanel;
}

class DetailPanel : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit DetailPanel(QWidget *parent = 0);
    ~DetailPanel();
public slots:
    void setCurrentNode(Node*);
    void setCurrentEdge(Edge*);
    void setCurrentPackage(PackageItem*);
    void setCurrentGenericItem(GenericMindMapItem* p);

    void updateCurrentNode();
    void updateCurrentEdge();
    void updateCurrentPackage();
    void updateGenericItem();
    void setAlwaysVisible(bool);
    
private:
    Ui::DetailPanel *ui;

    Node* m_currentNode;
    Edge* m_currentEdge;
    PackageItem* m_currentPackage;
    GenericMindMapItem* m_currentGenericItem;
    bool m_alwaysVisible;
};

#endif // DETAILPANEL_H

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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QFileInfo>

#include "graphwidget.h"
#include "mindmapview.h"
#include "mindtoolbar.h"
#include "preferences/preferencesmanager.h"

#include "browser/itembrowser.h"
#include "stringmanager/stringmanager.h"

#include "widgets/timelinewidget.h"
class DetailPanel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    void setupUi();

    void readSettings();
    void writeSettings();

    
    void readFileText();
signals:
    
public slots:
    void openRecentFile();
    void addNodeAt(QPoint pos);


protected:
    void closeEvent ( QCloseEvent * event );


private slots:
    void openMindMap();
    void newMindMap();
    void saveMindMap();
    void saveAsMindMap();
    void exportMap();

private: //function
    bool maybeSave();
    void makeMenu();
    void makeAction();
    void refreshOpenedFile();
    void updateTitle();
    void readFile();
    void addRecentFile();
    void displayDetailsPanel();
    void hideDetailsPanel();


private://attributs
    MindMapView* m_mindmapView;
    MindToolBar* m_mindtoolbar;
    QString m_title;

    QMenu* m_fileMenu;
    QMenu* m_exportMenu;
    QMenu* m_viewMenu;

    QAction* m_openAct;
    QAction* m_saveAct;
    QAction* m_saveAsAct;
    QAction* m_newAct;
    QAction* m_exitAct;

    QAction* m_svgExportAct;
    QAction* m_pngExportAct;

    QAction* m_zoomInAct;
    QAction* m_zoomOutAct;

    QAction* m_toolbarView;
    QAction* m_detailView;
    QAction* m_scenePropertiesAct;

    QAction* m_keepDetailsVisible;

    QList<QAction*>* m_recentFileActions;

   QDockWidget* m_dock;

   PreferencesManager* m_preferences;
   QString m_currentMindMapPath;

   DetailPanel* m_detailpanel;
   ItemBrowser* m_browser;
   StringManager* m_stringManager;
   TimeLineWidget* m_timeLine;

   MindMap* m_scene;


    
};

#endif // MAINWINDOW_H

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
#include <QDockWidget>
#include <QMenuBar>
#include <QFileDialog>
#include <QDebug>
#include <QDockWidget>
#include <QJsonDocument>
#include <QJsonObject>


#include "mainwindow.h"
#include "detailpanel/detailpanel.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    m_stringManager = new StringManager();
    m_mindmapView = new MindMapView(this);// new GraphWidget(m_stringManager,this);
    m_mindtoolbar = new MindToolBar(m_stringManager,this);
    //m_detailpanel = new DetailPanel(this);
    m_browser = new ItemBrowser;
    m_scene = new MindMap(this);
    m_scene->setStringManager(m_stringManager);
    m_mindmapView->setScene(m_scene);

    m_scene->setSceneRect(0,0,1024,768);
    m_mindmapView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    m_mindmapView->setRenderHints(QPainter::Antialiasing |QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform );
   /* m_timeLine = new TimeLineWidget();


    QDockWidget* bottom = new QDockWidget(tr("TimeLine"),this);
    bottom->setWidget(m_timeLine);
    addDockWidget(Qt::BottomDockWidgetArea,bottom);*/


    m_title = tr("%1[*] - MindMap Rolisteam - v0.0.1");
    setObjectName("MainWindow");
    setWindowTitle(m_title.arg(tr("untitled")));

    /*connect(m_mindmapView,SIGNAL(currentEdge(GenericMindMapItem*)),m_detailpanel,SLOT(setCurrentGenericItem(GenericMindMapItem*)));
    connect(m_mindmapView,SIGNAL(currentNode(GenericMindMapItem*)),m_detailpanel,SLOT(setCurrentGenericItem(GenericMindMapItem*)));
    connect(m_mindmapView,SIGNAL(currentPackage(GenericMindMapItem*)),m_detailpanel,SLOT(setCurrentGenericItem(GenericMindMapItem*)));

    connect(m_mindmapView,SIGNAL(currentPackage(GenericMindMapItem*)),m_detailpanel,SLOT(show()));
    connect(m_mindmapView,SIGNAL(currentEdge(GenericMindMapItem*)),m_detailpanel,SLOT(show()));
    connect(m_mindmapView,SIGNAL(currentNode(GenericMindMapItem*)),m_detailpanel,SLOT(show()));
    connect(m_mindmapView,SIGNAL(selectionIsEmpty()),m_detailpanel,SLOT(hide()));
    connect(m_mindmapView,SIGNAL(nodeAsBrush(Node*)),m_mindtoolbar,SLOT(addNodeBrush(Node*)));

    connect(m_mindmapView,SIGNAL(itemHasBeenAdded(GenericMindMapItem*)),m_browser,SLOT(addItem(GenericMindMapItem*)));

    connect(m_mindmapView,SIGNAL(itemHasBeenDeleted(GenericMindMapItem*)),m_browser,SLOT(removeItem(GenericMindMapItem*)));*/


    m_preferences = PreferencesManager::getInstance();

    m_recentFileActions = new QList<QAction*>();

    setupUi();

    readSettings();
    makeAction();
    makeMenu();

}

void MainWindow::closeEvent ( QCloseEvent * event )
{
    if (maybeSave())
    {
        writeSettings();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}
bool MainWindow::maybeSave()
{
    return true;
}
void MainWindow::hideDetailsPanel()
{
  /*  if(!m_keepDetailsVisible->isChecked())
    {
        m_detailpanel->hide();
    }*/

}
void MainWindow::displayDetailsPanel()
{
//    m_detailpanel->show();

}
void MainWindow::setupUi()
{
    setCentralWidget(m_mindmapView);
   /* m_dock = new QDockWidget(this);
    m_dock->setWidget(m_mindtoolbar);
    m_dock->setObjectName("ToolBar");
    m_dock->setWindowTitle(tr("ToolBar"));

    addDockWidget(Qt::LeftDockWidgetArea,m_dock);

    m_dock = new QDockWidget(this);
    m_dock->setWidget(m_browser);
    m_dock->setObjectName("Browser");
    m_dock->setWindowTitle(tr("Item Browser"));

    addDockWidget(Qt::LeftDockWidgetArea,m_dock);
    addDockWidget(Qt::RightDockWidgetArea,m_detailpanel);*/
}


void MainWindow::readSettings()
{
    QSettings settings("rolisteam","mapmind/preferences");

    move(settings.value("pos", QPoint(200, 200)).toPoint());
    resize(settings.value("size", QSize(600, 400)).toSize());
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());

    int size = settings.beginReadArray("recentFiles");
    for (int i = size-1; i > -1; --i)
    {
         settings.setArrayIndex(i);
         QFile info(settings.value("path").toString());
         if(info.exists())
         {
             QAction* tmp=new QAction(settings.value("name").toString(),this);
             tmp->setData(settings.value("path"));
             m_recentFileActions->append(tmp);
         }
     }
     settings.endArray();


    m_mindtoolbar->readSettings(settings);
    m_preferences->readSettings(settings);
}

void MainWindow::writeSettings()
{
    QSettings settings("rolisteam","mapmind/preferences");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());

    settings.beginWriteArray("recentFiles");
    for (int i = 0; i < m_recentFileActions->size(); ++i)
    {
        settings.setArrayIndex(i);
        settings.setValue("name", m_recentFileActions->at(i)->text());
        settings.setValue("path", m_recentFileActions->at(i)->data());
    }
    settings.endArray();

    m_mindtoolbar->writeSettings(settings);
    m_preferences->writeSettings(settings);
}
void MainWindow::makeMenu()
{

    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_newAct);
    m_fileMenu->addAction(m_openAct);
    m_fileMenu->addAction(m_saveAct);
    m_fileMenu->addAction(m_saveAsAct);
    m_fileMenu->addSeparator();
    m_exportMenu =  m_fileMenu->addMenu(tr("&Export"));
    m_exportMenu->addAction(m_svgExportAct);
    m_exportMenu->addAction(m_pngExportAct);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_scenePropertiesAct);
    m_fileMenu->addSeparator();
    refreshOpenedFile();
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAct);





    m_viewMenu = menuBar()->addMenu(tr("&View"));
    m_viewMenu->addAction(m_zoomInAct);
    m_viewMenu->addAction(m_zoomOutAct);
    m_viewMenu->addSeparator();
    m_viewMenu->addAction(m_detailView);
    m_viewMenu->addAction(m_toolbarView);
}
void MainWindow::refreshOpenedFile()
{
    for (int i = 0; i < m_recentFileActions->size(); ++i)
    {
      m_fileMenu->addAction(m_recentFileActions->at(i));
      connect(m_recentFileActions->at(i),SIGNAL(triggered()),this,SLOT(openRecentFile()));
    }
}
void MainWindow::makeAction()
{
    m_newAct = new QAction( tr("&New"), this);//QIcon(":/pixmaps/filenew.xpm"),
    m_newAct->setShortcut(tr("Ctrl+N"));
    m_newAct->setStatusTip(tr("Create a new file"));
    connect(m_newAct, SIGNAL(triggered()), this, SLOT(newMindMap()));

    m_openAct = new QAction(tr("&Open..."), this);
    m_openAct->setShortcut(tr("Ctrl+O"));
    m_openAct->setStatusTip(tr("Open an existing file"));
    connect(m_openAct, SIGNAL(triggered()), this, SLOT(openMindMap()));

    m_saveAct = new QAction(tr("&Save"), this);
    m_saveAct->setShortcut(tr("Ctrl+S"));
    m_saveAct->setStatusTip(tr("Save the document to disk"));
    connect(m_saveAct, SIGNAL(triggered()), this, SLOT(saveMindMap()));

    m_saveAsAct = new QAction(tr("Save &As..."), this);
    m_saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(m_saveAsAct, SIGNAL(triggered()), this, SLOT(saveAsMindMap()));



    m_scenePropertiesAct = new QAction(tr("Scene Properties"),this);
    //connet(m_scenePropertiesAct);

    m_exitAct = new QAction(tr("E&xit"), this);
    m_exitAct->setShortcut(tr("Ctrl+Q"));
    m_exitAct->setStatusTip(tr("Exit the application"));
    connect(m_exitAct, SIGNAL(triggered()), this, SLOT(close()));

    m_svgExportAct= new QAction(tr("Svg"), this);
    m_svgExportAct->setStatusTip(tr("Export the map as SVG file"));
    connect(m_svgExportAct, SIGNAL(triggered()), this, SLOT(exportMap()));

    m_pngExportAct= new QAction(tr("Image"), this);
    m_pngExportAct->setStatusTip(tr("Export the map as Image file"));
    connect(m_pngExportAct, SIGNAL(triggered()), this, SLOT(exportMap()));

    m_zoomInAct = new QAction(tr("Zoom In"),this);
    m_zoomInAct->setShortcut(tr("Ctrl++"));
    m_zoomInAct->setStatusTip(tr("Zoom in the map"));
    connect(m_zoomInAct,SIGNAL(triggered()),m_mindmapView,SLOT(zoomIn()));


    m_zoomOutAct = new QAction(tr("Zoom Out"),this);
    m_zoomOutAct->setStatusTip(tr("Zoom out of the map"));
    m_zoomOutAct->setShortcut(tr("Ctrl+-"));
    connect(m_zoomOutAct,SIGNAL(triggered()),m_mindmapView,SLOT(zoomOut()));

    m_detailView = new QAction(tr("Show/Hide Detail view"),this);

    m_toolbarView = new QAction(tr("Show/Hide Toolbar"),this);

    m_keepDetailsVisible = new QAction(tr("Always show detail view"),this);
    m_keepDetailsVisible->setCheckable(true);
    m_keepDetailsVisible->setChecked(true);
    m_toolbarView->setCheckable(true);
    //connect(m_toolbarView,SIGNAL(toggled(bool)),m_dock,SLOT(setVisible(bool)));
}
void MainWindow::updateTitle()
{
    if(m_currentMindMapPath.isNull())
    {
        setWindowTitle(m_title.arg(tr("untitled")));
    }
    else
    {
        QFileInfo info(m_currentMindMapPath);

        setWindowTitle(m_title.arg(info.fileName()));
    }
}
void MainWindow::openRecentFile()
{
    QAction* tmp = dynamic_cast<QAction*>(sender());
    m_currentMindMapPath=tmp->data().toString();

    readFile();
    updateTitle();

}

void MainWindow::addRecentFile()
{
    QFileInfo fileinfo(m_currentMindMapPath);
    m_preferences->registerValue("MindMapDirectory",fileinfo.absoluteDir().canonicalPath());
    QAction* tmp=new QAction(fileinfo.fileName(),this);
    tmp->setData(fileinfo.absoluteFilePath());
    connect(tmp,SIGNAL(triggered()),this,SLOT(openRecentFile()));

    int size=m_preferences->value("MaxSizeRecentFile",5).toInt();
    if((size<=m_recentFileActions->size())&&(size>0))
    {
        delete m_recentFileActions->at(0);
        m_recentFileActions->removeAt(0);
    }
    m_recentFileActions->append(tmp);
}
void MainWindow::readFile()
{
    QFile file(m_currentMindMapPath);
    if (file.open(QIODevice::ReadOnly))
    {
        QJsonDocument json = QJsonDocument::fromJson(file.readAll());
        QJsonObject jsonObj = json.object();


        //m_stringManager->readFromData(jsonObj);
        m_scene->readFromData(jsonObj);
        file.close();
    }
}

/*void MainWindow::readFile()
{
    QFile file(m_currentMindMapPath);
    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream in(&file);
        m_stringManager->readFromData(in);
        //m_widget->readFromData(in);
        file.close();
    }
}*/
void MainWindow::readFileText()
{
    QFile file(m_currentMindMapPath);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        //m_stringManager->readFromData(in);
        m_scene->readFromText(in);
        file.close();
    }
}

void MainWindow::openMindMap()
{
    m_currentMindMapPath = QFileDialog::getOpenFileName(this, tr("Open Mind Map"), m_preferences->value("MindMapDirectory",QDir::homePath()).toString(), tr("Rolisteam Mind Map (*.rmap *.txt)"));
    if(!m_currentMindMapPath.isNull())
    {
        if(m_currentMindMapPath.endsWith(".rmap"))
        {
            readFile();
            addRecentFile();
            updateTitle();
        }
        else if(m_currentMindMapPath.endsWith(".txt"))
        {
            readFileText();
            addRecentFile();
            updateTitle();
        }

    }
}
void MainWindow::addNodeAt(QPoint pos)
{
    Node* root = new Node(this);
    m_scene->addItem(root);
    //m_nodeList->append(node);
    root->setPos(pos.x(),pos.y());
    root->setStringManager(m_stringManager);

    root->setColorTheme(m_preferences->getDefaultNodeColorTheme());
    root->setText("Root");

    m_scene->appendRoot(root);
}
void MainWindow::newMindMap()
{
    //m_mindmapView->cleanScene();
    m_browser->clear();
}

void MainWindow::saveMindMap()
{

    if (m_currentMindMapPath.isNull())
    {
            saveAsMindMap();
    }

    if(!m_currentMindMapPath.endsWith(".rmap"))
    {
         m_currentMindMapPath += ".rmap";
    }
    QFile file(m_currentMindMapPath);


    QFileInfo fileinfo(file);
    m_preferences->registerValue("MindMapDirectory",fileinfo.absoluteDir().canonicalPath());

    QJsonDocument doc;
    QJsonObject root;
    //m_stringManager->writeToData(root);
    m_scene->writeToData(root);

    doc.setObject(root);


    //QFile file(m_currentMindMapPath);
    if (file.open(QIODevice::WriteOnly))
    {
        QFileInfo fileinfo(file);
        m_preferences->registerValue("MindMapDirectory",fileinfo.absoluteDir().canonicalPath());
        file.write(doc.toJson());
        file.close();
    }
}
void MainWindow::saveAsMindMap()
{
    m_currentMindMapPath = QFileDialog::getSaveFileName(this, tr("Save Mind Map"), m_preferences->value("MindMapDirectory",QDir::homePath()).toString(), tr("Rolisteam Mind Map (*.rmap)"));
    if(!m_currentMindMapPath.isNull())
    {
        saveMindMap();
        updateTitle();    
    }
}
void MainWindow::exportMap()
{
    QObject* obj = sender();

    if(obj==m_svgExportAct)
    {
        QString uri = QFileDialog::getSaveFileName(this, tr("export Mind Map as SVG"), m_preferences->value("MindMapDirectory",QDir::homePath()).toString(), tr("SVG file (*.svg)"));

        //m_mindmapView->dumpMapInSvg(uri);

    }
    else if(obj == m_pngExportAct)
    {
        QString uri = QFileDialog::getSaveFileName(this, tr("export Mind Map as Image"), m_preferences->value("MindMapDirectory",QDir::homePath()).toString(), tr("Bipmap file (*.png, *.jpg, *.bmp)"));
        m_scene->dumpMapInBipmap(uri);
    }
}

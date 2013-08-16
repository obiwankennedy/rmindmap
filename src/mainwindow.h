#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QFileInfo>

#include "graphwidget.h"
#include "mindtoolbar.h"
#include "preferences/preferencesmanager.h"

#include "browser/itembrowser.h"
#include "stringmanager/stringmanager.h"


class DetailPanel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    void setupUi();

    void readSettings();
    void writeSettings();

    
signals:
    
public slots:
    void openRecentFile();


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


private://attributs
    GraphWidget* m_widget;
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
    QAction* m_bipmapExportAct;

    QAction* m_zoomInAct;
    QAction* m_zoomOutAct;

    QAction* m_toolbarView;
    QAction* m_detailView;

    QList<QAction*>* m_recentFileActions;


   QDockWidget* m_dock;

   PreferencesManager* m_preferences;
   QString m_currentMindMapPath;

   DetailPanel* m_detailpanel;
   ItemBrowser* m_browser;
   StringManager* m_stringManager;
    
};

#endif // MAINWINDOW_H

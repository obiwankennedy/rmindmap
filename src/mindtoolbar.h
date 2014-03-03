#ifndef MINDTOOLBAR_H
#define MINDTOOLBAR_H

#include <QWidget>
#include <QPushButton>
#include <QListView>
#include <QSettings>
#include <QButtonGroup>


#include "serializable.h"
#include "graphitemmodel.h"
#include "graphedgemodel.h"
#include "graphcommonmodel.h"
#include "stringmanager/stringmanager.h"

class MindToolBar : public QWidget, public SettingsUser
{
    Q_OBJECT
public:
    enum MINDTOOL { NONE, HANDLE,ADD_ITEM,DEL_ITEM, ARROW, ADD_BREAK, DEL_BREAK , ADD_GEO};
    explicit MindToolBar(StringManager* stringManager,QWidget *parent = 0);

    void setupUi();

    void readSettings(QSettings& settings);
    void writeSettings(QSettings& settings);


signals:
    void currentNodeBrushChanged(Node*);
    void currentEdgeBrushChanged(Edge*);
    void currentGeoTypeChanged(QString);
    void currentToolChanged(MindToolBar::MINDTOOL);

public slots:
    void addNodeBrush(Node*);
    void currentToolHasChanged(MindToolBar::MINDTOOL);

private slots:
    void clickOn(QModelIndex );
    void clickOnEdge(QModelIndex );
    void clickOnCommon(QModelIndex );
    void chooseTool();





private:
    //list
    QListView* m_itemList;
    QListView* m_edgeList;
    QListView* m_geoList;

    GraphItemModel* m_model;
    GraphEdgeModel* m_edgeModel;
    GraphCommonModel* m_commonModel;
    

    //buttons
    QPushButton* m_handleItem;

    QPushButton* m_addItem;
    QPushButton* m_delItem;

    QPushButton* m_arrow;
    QPushButton* m_addBreak;
    QPushButton* m_delBreak;

    QButtonGroup* m_group;

    StringManager* m_stringManager;

};

#endif // MINDTOOLBAR_H

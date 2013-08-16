#ifndef ITEMBROWSER_H
#define ITEMBROWSER_H



#include <QWidget>
#include <QTreeView>

#include <QAction>

#include "itembrowsermodel.h"
class QVBoxLayout;

class ItemBrowser : public QWidget
{
    Q_OBJECT

public:
    ItemBrowser();


public slots:
    void addItem(GenericMindMapItem*);
    void removeItem(GenericMindMapItem*);
    void clear();

signals:
    void selectionChanged(GenericMindMapItem*);
    void itemHasBeenDeleted(GenericMindMapItem* item);

private slots:
    void contextMenu(const QPoint p);
    void removeCurrentItem();
private:
    QTreeView* m_view;
    ItemBrowserModel* m_model;
    QVBoxLayout* m_layout;
    QAction* m_removeItem;

};

#endif // ITEMBROWSER_H

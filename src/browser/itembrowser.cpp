#include <QVBoxLayout>
#include <QMenu>

#include "itembrowser.h"



ItemBrowser::ItemBrowser()
{
    m_model = new ItemBrowserModel();
    m_view = new QTreeView(this);
    m_view->setContextMenuPolicy(Qt::CustomContextMenu);
    m_view->setHeaderHidden(true);
    m_layout = new QVBoxLayout();
    m_layout->addWidget(m_view);
    m_view->setModel(m_model);
    setLayout(m_layout);


    m_removeItem = new QAction(tr("Remove Items"),this);
    connect(m_view,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenu(QPoint)));
    connect(m_removeItem,SIGNAL(triggered()),this,SLOT(removeCurrentItem()));
}
void ItemBrowser::addItem(GenericMindMapItem* p)
{
    m_model->insertItem(p);
}

void ItemBrowser::removeItem(GenericMindMapItem*)
{

}

void ItemBrowser::clear()
{
    m_model->clear();
}
void ItemBrowser::contextMenu(const QPoint p)
{
    QMenu custom;

    custom.addAction(m_removeItem);

    custom.exec(mapToGlobal(p));
}
void ItemBrowser::removeCurrentItem()
{
    GenericMindMapItem* item = m_model->indexToMindMapItem(m_view->currentIndex());
    if(NULL!=item)
    {
        emit itemHasBeenDeleted(item);

    }
    m_model->removeItem(m_view->currentIndex());

}

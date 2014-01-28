#ifndef DETAILPANEL_H
#define DETAILPANEL_H

#include <QDockWidget>


#include "node.h"
#include "edge.h"
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
    
private:
    Ui::DetailPanel *ui;

    Node* m_currentNode;
    Edge* m_currentEdge;
    PackageItem* m_currentPackage;
};

#endif // DETAILPANEL_H

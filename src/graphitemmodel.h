#ifndef GRAPHITEMMODEL_H
#define GRAPHITEMMODEL_H

#include <QAbstractListModel>
#include <QSettings>

#include "node.h"
class PreferencesManager;

class GraphItemModel : public QAbstractListModel
{
public:
    GraphItemModel();
    ~GraphItemModel();
    int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    void addItems(Node* node);

    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags ( const QModelIndex & index ) const;


    Node* getNodeAt(const QModelIndex & index);



    void readSettings(QSettings& settings);
    void writeSettings(QSettings& settings);


private:
    QList<Node*>* m_list;
    PreferencesManager* m_preferences;

};

#endif // GRAPHITEMMODEL_H

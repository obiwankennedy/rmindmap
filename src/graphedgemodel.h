#ifndef GRAPHEDGEMODEL_H
#define GRAPHEDGEMODEL_H

#include <QAbstractListModel>
#include <QSettings>

#include "edge.h"
#include "serializable.h"

class GraphEdgeModel : public QAbstractListModel, public SettingsUser
{
public:
    GraphEdgeModel();
    ~GraphEdgeModel();
    int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    void addItems(Edge* node);



    Edge* getNodeAt(const QModelIndex & index);



    void readSettings(QSettings& settings);
    void writeSettings(QSettings& settings);




private:
    QList<Edge*>* m_list;
};

#endif // GRAPHEDGEMODEL_H

#ifndef GRAPHCOMMONMODEL_H
#define GRAPHCOMMONMODEL_H

#include <QAbstractListModel>

#include "serializable.h"

class PackageItem;

class GraphCommonModel : public QAbstractListModel, public SettingsUser
{
public:
    GraphCommonModel();
    ~GraphCommonModel();
    int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    void addItems(PackageItem* node);



    PackageItem* getNodeAt(const QModelIndex & index);



    void readSettings(QSettings& settings);
    void writeSettings(QSettings& settings);


private:
    QList<PackageItem*>* m_list;
};

#endif // GRAPHCOMMONMODEL_H

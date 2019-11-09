#ifndef LINKMODEL_H
#define LINKMODEL_H

#include <QAbstractItemModel>

#include "data/link.h"

class LinkModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum Roles : int
    {
        // Curves
        Direction= Qt::UserRole + 1,
        Position,
        Last,
        Width,
        Height,
        LinkRole
    };
    explicit LinkModel(QObject* parent= nullptr);

    // Basic functionality:
    QModelIndex index(int row, int column, const QModelIndex& parent= QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent= QModelIndex()) const override;
    int columnCount(const QModelIndex& parent= QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role= Qt::DisplayRole) const override;
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role= Qt::EditRole) override;

    QHash<int, QByteArray> roleNames() const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    std::vector<Link*>& getDataSet();

    Link* addLink(MindNode* p1, MindNode* p2);
    void openLinkAndChildren(const QString& id, bool status);

    void clear();

    void append(Link* link);
    void removeLink(Link* link);
public slots:
    void linkHasChanged();

private:
    std::vector<Link*> m_data;
};

#endif // LINKMODEL_H

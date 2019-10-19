#ifndef BOXMODEL_H
#define BOXMODEL_H

#include <QAbstractListModel>

#include "data/mindnode.h"
#include "model/linkmodel.h"

class BoxModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(QAbstractItemModel* linkModel READ linkModel WRITE setLinkModel NOTIFY linkModelChanged)
public:
    enum Roles : int
    {
        // Curves
        Label= Qt::UserRole + 1,
        Color,
        ContentWidth,
        ContentHeight,
        Position,
        Node,
        Posx,
        Posy
    };
    explicit BoxModel(QObject* parent= nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex& parent= QModelIndex()) const override;
    int columnCount(const QModelIndex& parent= QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    QVariant data(const QModelIndex& index, int role= Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex& index, const QVariant& value, int role= Qt::EditRole) override;
    QModelIndex index(int row, int column, const QModelIndex& parent) const override;

    QHash<int, QByteArray> roleNames() const override;
    QAbstractItemModel* linkModel() const;
    void setLinkModel(QAbstractItemModel* linkModel);
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    std::vector<MindNode*>& nodes();

    void clear();
    void appendNode(MindNode* node);
signals:
    void linkModelChanged();
public slots:
    // Add data:
    bool addBox(const QString& idparent);

    // Remove data:
    bool removeBox(const QString& id);
    void openNode(const QString& id, bool status);

private:
    std::vector<MindNode*> m_data;
    LinkModel* m_linkModel= nullptr;
};

#endif // BOXMODEL_H

/***************************************************************************
 *	Copyright (C) 2019 by Renaud Guezennec                                 *
 *   http://www.rolisteam.org/contact                                      *
 *                                                                         *
 *   This software is free software; you can redistribute it and/or modify *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef MINDITEMMODEL_H
#define MINDITEMMODEL_H

#include <QAbstractListModel>

#include "data/minditem.h"
#include "data/mindnode.h"
#include "data/packagenode.h"

class MindItemModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QRectF contentRect READ contentRect NOTIFY contentRectChanged)
public:
    enum Roles
    {
        // Curves
        Label= Qt::UserRole + 1,
        Visible,
        Selected,
        Type,
        Uuid,
        Object
    };
    Q_ENUM(Roles)
    explicit MindItemModel(QObject* parent= nullptr);
    ~MindItemModel() override;

    // Basic functionality:
    int rowCount(const QModelIndex& parent= QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role= Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex& index, const QVariant& value, int role= Qt::EditRole) override;

    QHash<int, QByteArray> roleNames() const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    std::vector<MindItem*>& items(MindItem::Type type);

    void clear();
    void appendItem(MindItem* node);
    MindItem* item(const QString& id) const;

    QRectF contentRect() const;

    std::vector<Link*> sublink(const QString& id);

    std::vector<PositionedItem*> positionnedItems() const;

public slots:
    // Add data:
    std::pair<MindItem*, Link*> addItem(const QString& idparent, MindItem::Type type);

    // Remove data:
    bool removeItem(const MindItem* node);
    void openItem(const QString& id, bool status);
    void setImageUriToNode(const QString& id, const QString& url);

signals:
    void contentRectChanged();

private:
    std::vector<MindItem*> m_links;
    std::vector<MindItem*> m_packages;
    std::vector<MindItem*> m_nodes;
};

#endif // MINDITEMMODEL_H

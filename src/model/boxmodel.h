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
    ~BoxModel() override;

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
    std::pair<MindNode*, Link*> addBox(const QString& idparent);

    // Remove data:
    bool removeBox(const MindNode* node);
    void openNode(const QString& id, bool status);

private:
    std::vector<MindNode*> m_data;
    LinkModel* m_linkModel= nullptr;
};

#endif // BOXMODEL_H

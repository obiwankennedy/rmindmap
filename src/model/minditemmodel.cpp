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
#include "minditemmodel.h"

#include <QColor>
#include <QDebug>
#include <QRectF>

#include "model/nodeimageprovider.h"

std::vector<MindItem*>& getVector(std::vector<MindItem*>& links, std::vector<MindItem*>& package,
                                  std::vector<MindItem*>& node, MindItem::Type type)
{
    if(type == MindItem::LinkType)
        return links;
    else if(type == MindItem::PackageType)
        return package;
    else // if(type == MindItem::NodeType)
        return node;
}

MindItemModel::MindItemModel(ImageModel* imgModel, QObject* parent) : QAbstractListModel(parent), m_imgModel(imgModel)
{
}

MindItemModel::~MindItemModel() {}

int MindItemModel::rowCount(const QModelIndex& parent) const
{
    if(!parent.isValid())
    {
        // qDebug() << "links" << m_links.size() << "node:" << m_nodes.size();
        return static_cast<int>(m_links.size() + m_packages.size() + m_nodes.size());
    }

    return 0;
}

QVariant MindItemModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    QVariant result;

    auto r= index.row();
    auto linkCount= static_cast<int>(m_links.size());
    auto packageCount= static_cast<int>(m_packages.size());
    auto nodeCount= static_cast<int>(m_nodes.size());

    MindItem* mindNode= nullptr;

    if(r < linkCount)
    {
        mindNode= m_links[r];
    }
    else
    {
        r-= linkCount;
        if(r < packageCount)
            mindNode= m_packages[r];
        else
        {
            r-= packageCount;
            if(r < nodeCount)
                mindNode= m_nodes[r];
        }
    }
    if(!mindNode)
        return {};

    switch(role)
    {
    case Visible:
        result= mindNode->isVisible();
        break;
    case Label:
        result= mindNode->text();
        break;
    case Selected:
        result= mindNode->selected();
        break;
    case Type:
        result= mindNode->type();
        break;
    case Uuid:
        result= mindNode->id();
        break;
    case Object:
        result= QVariant::fromValue(mindNode);
        break;
    case HasPicture:
        result= mindNode->type() == MindItem::NodeType ? m_imgModel->hasPixmap(mindNode->id()) : false;
        break;
    }
    return result;
}

bool MindItemModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(data(index, role) != value)
    {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

void MindItemModel::update(const QString& id, int role)
{
    auto current= item(id);

    if(!current)
        return;

    int row= 0;
    if(current->type() == MindItem::LinkType)
    {
        auto it= std::find(std::begin(m_links), std::end(m_links), current);
        if(it != std::end(m_links))
            row= std::distance(std::begin(m_links), it);
    }
    else if(current->type() == MindItem::PackageType)
    {
        auto it= std::find(std::begin(m_packages), std::end(m_packages), current);
        if(it != std::end(m_packages))
            row= std::distance(std::begin(m_packages), it) + m_links.size();
    }
    else if(current->type() == MindItem::NodeType)
    {
        auto it= std::find(std::begin(m_nodes), std::end(m_nodes), current);
        if(it != std::end(m_nodes))
            row= std::distance(std::begin(m_nodes), it) + m_links.size() + m_packages.size();
    }

    qDebug() << row << role << "update";
    emit dataChanged(index(row, 0), index(row, 0), {role});
}

void MindItemModel::setImageUriToNode(const QString& id, const QString& url)
{
    auto it= std::find_if(m_nodes.begin(), m_nodes.end(), [id](const MindItem* node) { return node->id() == id; });
    if(it == m_nodes.end())
        return;

    auto dis= std::distance(m_nodes.begin(), it);
    auto node= dynamic_cast<MindNode*>(*it);
    if(!node)
        return;

    node->setImageUri(url);
    auto idx= index(dis, 0, QModelIndex());
    emit dataChanged(idx, idx, QVector<int>());
}

Qt::ItemFlags MindItemModel::flags(const QModelIndex& index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}

QHash<int, QByteArray> MindItemModel::roleNames() const
{
    static QHash<int, QByteArray> roles= {{MindItemModel::Label, "label"},
                                          {MindItemModel::Visible, "visible"},
                                          {MindItemModel::Selected, "selected"},
                                          {MindItemModel::Type, "type"},
                                          {MindItemModel::Uuid, "id"},
                                          {MindItemModel::Object, "object"},
                                          {MindItemModel::HasPicture, "hasPicture"}};
    return roles;
}

void MindItemModel::clear()
{

    QVector<MindItem*> backup;
    backup.reserve(rowCount());
    for(auto const& l : m_links)
        backup.append(l);

    for(auto const& l : m_packages)
        backup.append(l);

    for(auto const& l : m_nodes)
        backup.append(l);

    beginResetModel();
    m_links.clear();
    m_packages.clear();
    m_nodes.clear();
    endResetModel();
}

void MindItemModel::appendItem(MindItem* node)
{
    if(node == nullptr)
        return;

    auto& vec= getVector(m_links, m_packages, m_nodes, node->type());

    int row= vec.size();

    switch(node->type())
    {
    case MindItem::LinkType:
    {
        auto link= dynamic_cast<LinkController*>(node);
        if(link)
            connect(link, &LinkController::startPointChanged, this, [this, link]() {
                QModelIndex parent;
                auto it= std::find(m_links.begin(), m_links.end(), link);
                if(it == m_links.end())
                    return;
                auto offset= std::distance(m_links.begin(), it);
                auto idx1= index(offset, 0, parent);
                emit dataChanged(idx1, idx1, QVector<int>());
            });
    }
    break;
    case MindItem::PackageType:
        row+= m_links.size();
        break;
    case MindItem::NodeType:
        row+= m_links.size();
        row+= m_packages.size();
        break;
    }

    beginInsertRows(QModelIndex(), row, row);
    vec.push_back(node);
    endInsertRows();
}
std::vector<PositionedItem*> MindItemModel::positionnedItems() const
{
    std::vector<PositionedItem*> vec;
    vec.reserve(m_packages.size() + m_nodes.size());

    for(auto const& item : m_packages)
    {
        auto pack= dynamic_cast<PositionedItem*>(item);

        if(!pack)
            continue;

        vec.push_back(pack);
    }

    for(auto const& item : m_nodes)
    {
        auto pack= dynamic_cast<PositionedItem*>(item);

        if(!pack)
            continue;

        vec.push_back(pack);
    }

    return vec;
}

std::pair<MindItem*, LinkController*> MindItemModel::addItem(const QString& idparent, MindItem::Type type)
{

    auto& vec= getVector(m_links, m_packages, m_nodes, type);

    auto row= static_cast<int>(vec.size());
    switch(type)
    {
    case MindItem::LinkType:
        break;
    case MindItem::PackageType:
        row+= m_links.size();
        break;
    case MindItem::NodeType:
        row+= m_links.size();
        row+= m_packages.size();
        break;
    }

    std::pair<MindItem*, LinkController*> result;

    if(type == MindItem::NodeType)
    {

        beginInsertRows(QModelIndex(), row, row);
        auto root= new MindNode();

        vec.push_back(root);

        if(idparent.isEmpty())
            return {};

        auto data= positionnedItems();

        auto id= std::find_if(data.begin(), data.end(),
                              [idparent](const PositionedItem* node) { return idparent == node->id(); });
        if(id == data.end())
            return {};

        auto rectParent= (*id)->boundingRect();
        auto pos= rectParent.topLeft() + QPointF(rectParent.width() * 1.5, rectParent.height() * 1.5);
        root->setPosition(pos);
        endInsertRows();

        emit contentRectChanged();

        auto link= new LinkController();
        link->setStart((*id));
        link->setEnd(root);
        root->setParentNode(*id);

        appendItem(link);

        result= std::make_pair(root, link);
    }
    else if(type == MindItem::PackageType)
    {
        beginInsertRows(QModelIndex(), row, row);
        auto pack= new PackageNode();

        vec.push_back(pack);

        endInsertRows();

        result= std::make_pair(pack, nullptr);
    }

    return result;
}

bool MindItemModel::removeItem(const MindItem* node)
{
    if(node == nullptr)
        return false;

    auto& vec= getVector(m_links, m_packages, m_nodes, node->type());
    auto it= std::find(vec.begin(), vec.end(), node);

    if(it == std::end(m_nodes))
        return false;

    auto idx= static_cast<int>(std::distance(vec.begin(), it));

    beginRemoveRows(QModelIndex(), idx, idx);
    vec.erase(it);
    endRemoveRows();
    return true;
}

void MindItemModel::openItem(const QString& id, bool status)
{
    auto it= item(id);

    if(nullptr == it)
        return;

    auto node= dynamic_cast<PositionedItem*>(it);

    if(nullptr == node)
        return;

    if(node->open() == status)
        return;

    node->setOpen(status);
}

MindItem* MindItemModel::item(const QString& id) const
{
    // MindItem* result= nullptr;

    auto it= std::find_if(m_nodes.begin(), m_nodes.end(), [id](const MindItem* node) { return node->id() == id; });

    if(it != m_nodes.end())
        return *it;

    auto it2= std::find_if(m_links.begin(), m_links.end(), [id](const MindItem* node) { return node->id() == id; });

    if(it2 != m_links.end())
        return *it2;

    auto it3
        = std::find_if(m_packages.begin(), m_packages.end(), [id](const MindItem* node) { return node->id() == id; });

    if(it3 != m_packages.end())
        return *it3;

    return nullptr;
}

QRectF MindItemModel::contentRect() const
{
    QRectF rect(0, 0, 0, 0);

    for(auto const& item : m_nodes)
    {
        auto node= dynamic_cast<PositionedItem*>(item);
        if(!node)
            continue;
        rect|= node->boundingRect();
    }

    for(auto const& item : m_packages)
    {
        auto pack= dynamic_cast<PositionedItem*>(item);
        if(!pack)
            continue;
        rect|= pack->boundingRect();
    }

    return rect;
}

std::vector<MindItem*>& MindItemModel::items(MindItem::Type type)
{
    return getVector(m_links, m_packages, m_nodes, type);
}

std::vector<LinkController*> MindItemModel::sublink(const QString& id)
{
    std::vector<LinkController*> vec;

    for(auto item : m_links)
    {
        auto link= dynamic_cast<LinkController*>(item);
        if(!link)
            continue;

        auto st= link->start();
        if(!st)
            continue;

        if(id == st->id())
            vec.push_back(link);
    }

    return vec;
}

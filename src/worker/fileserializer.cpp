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
#include "fileserializer.h"

#include "controller/mindmapcontroller.h"
#include "data/linkcontroller.h"
#include "data/mindnode.h"
#include "data/packagenode.h"
#include "model/minditemmodel.h"
#include "model/nodeimageprovider.h"

#include <QBuffer>
#include <QByteArray>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <random>

FileSerializer::FileSerializer() {}

bool FileSerializer::readTextFile(MindItemModel* nodeModel, const QString& filepath)
{
    QFile file(QUrl(filepath).path());
    std::random_device rd;
    std::mt19937 gen(rd());

    if(!file.open(QFile::ReadOnly))
    {
        return false;
    }

    QVector<MindNode*> parent;
    MindNode* previousNode= nullptr;
    int depth= 0;
    while(!file.atEnd())
    {
        QByteArray line= file.readLine();
        auto text= QString::fromUtf8(line);
        auto trimmed= text.trimmed();
        if(text.trimmed().isEmpty())
            continue;

        auto node= new MindNode();

        auto newdepth= std::max(0, (text.indexOf(trimmed[0])) / 2);

        node->setText(text.trimmed());
        std::uniform_real_distribution<> dist(0.0, 1600.0);
        node->setPosition({dist(gen), dist(gen)});
        nodeModel->appendItem(node);

        if(newdepth > depth && previousNode != nullptr)
        {
            parent.append(previousNode);
        }
        while(newdepth < depth)
        {
            parent.removeLast();
            depth-= 1;
        }

        if(newdepth >= depth && !parent.isEmpty())
        {
            // parent.last(), node;
            auto link= new LinkController();
            link->setStart(parent.last());
            link->setEnd(node);
            nodeModel->appendItem(link);
        }

        previousNode= node;
        depth= newdepth;
    }

    return true;
}

bool FileSerializer::readFile(MindMapController* controller, const QString& filepath)
{
    QFile file(filepath);
    if(!file.open(QFile::ReadOnly))
    {
        return false;
    }

    auto nodeModel= dynamic_cast<MindItemModel*>(controller->itemModel());

    auto data= file.readAll();
    QJsonDocument doc= QJsonDocument::fromJson(data);

    auto json= doc.object();

    auto linkArray= json["links"].toArray();
    auto imgArray= json["imgs"].toArray();
    auto nodeArray= json["nodes"].toArray();
    auto packagesArray= json["packages"].toArray();
    auto imgModel= controller->imgModel();
    for(auto const& imgRef : qAsConst(imgArray))
    {
        auto img= imgRef.toObject();
        auto id= img["id"].toString();
        auto data= QByteArray::fromBase64(img["data"].toString().toLocal8Bit());
        QPixmap map;
        if(map.loadFromData(data))
            imgModel->appendData(id, map);
    }

    std::map<QString, PositionedItem*> nodeMap;
    for(auto const& nodeRef : qAsConst(nodeArray))
    {
        auto obj= nodeRef.toObject();
        auto node= new MindNode();
        node->setId(obj["id"].toString());
        auto x= obj["x"].toDouble();
        auto y= obj["y"].toDouble();
        node->setPosition(QPointF(x, y));
        node->setText(obj["text"].toString());
        node->setImageUri(obj["image"].toString());
        node->setVisible(obj["visible"].toBool());
        node->setOpen(obj["open"].toBool());
        node->setStyleIndex(obj["styleIndex"].toInt());
        node->setDescription(obj["description"].toString());
        QStringList tags;
        auto tagArray= obj["tags"].toArray();
        std::transform(std::begin(tagArray), std::end(tagArray), std::back_inserter(tags),
                       [](const QJsonValue& val) { return val.toString(); });
        node->setTags(tags);

        nodeModel->appendItem(node);
        nodeMap.insert({node->id(), node});
    }

    for(auto const& packRef : qAsConst(packagesArray))
    {
        auto obj= packRef.toObject();
        auto node= new PackageNode();
        node->setId(obj["id"].toString());
        auto x= obj["x"].toDouble();
        auto y= obj["y"].toDouble();
        node->setPosition(QPointF(x, y));
        node->setText(obj["text"].toString());
        node->setVisible(obj["visible"].toBool());

        nodeModel->appendItem(node);
        nodeMap.insert({node->id(), node});
    }

    for(auto const& linkRef : qAsConst(linkArray))
    {
        auto obj= linkRef.toObject();
        // auto link= new Link();

        auto idStart= obj["idStart"].toString();
        auto idEnd= obj["idEnd"].toString();
        auto text= obj["text"].toString();
        auto color= obj["color"].toBool();
        auto constraint= obj["constraint"].toBool();

        auto link= new LinkController();
        link->setStart(nodeMap[idStart]);
        link->setEnd(nodeMap[idEnd]);
        nodeMap[idEnd]->setParentNode(nodeMap[idStart]);

        link->setText(text);
        link->setVisible(obj["visible"].toBool());
        link->setColor(color);
        link->setConstraint(constraint);
        link->setDirection(static_cast<LinkController::Direction>(obj["Direction"].toInt()));
        nodeModel->appendItem(link);
    }
    return true;
}

bool FileSerializer::writeFile(MindMapController* controller, const QString& filepath)
{
    QJsonObject data;

    // NODES
    QJsonArray nodeArray;
    auto nodeModel= dynamic_cast<MindItemModel*>(controller->itemModel());
    auto const& nodes= nodeModel->items(MindItem::NodeType);
    for(auto item : nodes)
    {
        auto node= dynamic_cast<MindNode*>(item);
        if(!node)
            continue;
        QJsonObject obj;
        obj["id"]= node->id();
        obj["x"]= node->position().x();
        obj["y"]= node->position().y();
        obj["text"]= node->text();
        obj["image"]= node->imageUri();
        obj["visible"]= node->isVisible();
        obj["open"]= node->open();
        obj["styleIndex"]= node->styleIndex();
        obj["description"]= node->description();
        obj["tags"]= QJsonArray::fromStringList(node->tags());
        nodeArray.append(obj);
    }
    data["nodes"]= nodeArray;

    // PACKAGES
    QJsonArray packageArray;
    auto const& packages= nodeModel->items(MindItem::PackageType);
    for(auto item : packages)
    {
        auto pack= dynamic_cast<PackageNode*>(item);
        if(!pack)
            continue;
        QJsonObject obj;
        obj["id"]= pack->id();
        obj["x"]= pack->position().x();
        obj["y"]= pack->position().y();
        obj["text"]= pack->text();

        obj["width"]= pack->width();
        obj["height"]= pack->height();
        obj["visible"]= pack->isVisible();
        packageArray.append(obj);
    }
    data["packages"]= packageArray;

    // LINKS
    QJsonArray linkArray;
    auto const& links= nodeModel->items(MindItem::LinkType);
    for(auto item : links)
    {
        auto link= dynamic_cast<LinkController*>(item);
        if(!link)
            continue;
        QJsonObject obj;
        obj["idStart"]= link->start()->id();
        obj["idEnd"]= link->end()->id();
        obj["visible"]= link->isVisible();
        obj["Direction"]= link->direction();
        obj["color"]= link->color();
        obj["constraint"]= link->constraint();
        obj["text"]= link->text();
        linkArray.append(obj);
    }
    data["links"]= linkArray;

    // Save image data

    auto imgModel= controller->imgModel();
    auto const& dataImg= imgModel->data();
    QJsonArray imgArray;
    for(auto& key : dataImg.keys())
    {
        QJsonObject img;
        img["id"]= key;

        auto pix= dataImg[key];
        QJsonArray linkArray;
        QByteArray bytes;
        {
            QBuffer buffer(&bytes);
            buffer.open(QIODevice::WriteOnly);
            pix.save(&buffer, "PNG");
        }
        img["data"]= QString(bytes.toBase64());
        imgArray.append(img);
    }
    data["imgs"]= imgArray;

    QJsonDocument doc;
    doc.setObject(data);

    QFile file(filepath);
    if(!file.open(QFile::WriteOnly))
    {
        return false;
    }
    file.write(doc.toJson());
    return true;
}

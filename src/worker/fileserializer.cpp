#include "fileserializer.h"

#include "model/boxmodel.h"
#include "model/linkmodel.h"

#include <QByteArray>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <random>

FileSerializer::FileSerializer() {}

bool FileSerializer::readTextFile(BoxModel* nodeModel, LinkModel* linkModel, const QString& filepath)
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
        nodeModel->appendNode(node);

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
            linkModel->addLink(parent.last(), node);
        }

        previousNode= node;
        depth= newdepth;
    }

    return true;
}
bool FileSerializer::readFile(BoxModel* nodeModel, LinkModel* linkModel, const QString& filepath)
{
    QFile file(filepath);
    if(!file.open(QFile::ReadOnly))
    {
        return false;
    }

    auto data= file.readAll();
    QJsonDocument doc= QJsonDocument::fromJson(data);

    auto json= doc.object();

    auto linkArray= json["links"].toArray();
    auto nodeArray= json["nodes"].toArray();

    std::map<QString, MindNode*> nodeMap;
    for(auto nodeRef : nodeArray)
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

        nodeModel->appendNode(node);
        nodeMap.insert({node->id(), node});
    }

    for(auto linkRef : linkArray)
    {
        auto obj= linkRef.toObject();
        // auto link= new Link();

        auto idStart= obj["idStart"].toString();
        auto idEnd= obj["idEnd"].toString();

        auto link= linkModel->addLink(nodeMap[idStart], nodeMap[idEnd]);
        link->setVisible(obj["visible"].toBool());
        link->setDirection(static_cast<Link::Direction>(obj["Direction"].toInt()));
    }
    return true;
}
bool FileSerializer::writeFile(BoxModel* nodeModel, LinkModel* linkModel, const QString& filepath)
{
    QJsonObject data;
    QJsonArray nodeArray;
    auto nodes= nodeModel->nodes();
    for(auto node : nodes)
    {
        QJsonObject obj;
        obj["id"]= node->id();
        obj["x"]= node->position().x();
        obj["y"]= node->position().y();
        obj["text"]= node->text();
        obj["image"]= node->imageUri();
        obj["visible"]= node->isVisible();
        obj["open"]= node->open();
        nodeArray.append(obj);
    }
    data["nodes"]= nodeArray;

    QJsonArray linkArray;
    auto links= linkModel->getDataSet();
    for(auto link : links)
    {
        QJsonObject obj;
        obj["idStart"]= link->start()->id();
        obj["idEnd"]= link->end()->id();
        obj["visible"]= link->isVisible();
        obj["Direction"]= link->direction();
    }
    data["links"]= linkArray;
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

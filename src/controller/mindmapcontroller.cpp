#include "mindmapcontroller.h"

#include "controller/spacingcontroller.h"
#include "model/boxmodel.h"
#include "model/linkmodel.h"

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>
#include <QUrl>
#include <random>

MindMapController::MindMapController(QObject* parent)
    : QObject(parent), m_linkModel(new LinkModel()), m_nodeModel(new BoxModel())
{
    m_nodeModel->setLinkModel(m_linkModel.get());

    m_spacing= new QThread();
    m_spacingController.reset(new SpacingController(m_nodeModel->nodes(), m_linkModel.get()));
    m_spacingController->moveToThread(m_spacing);
    connect(m_spacing, &QThread::started, m_spacingController.get(), &SpacingController::computeInLoop);
    connect(m_spacingController.get(), &SpacingController::runningChanged, this, &MindMapController::spacingChanged);
    connect(m_spacingController.get(), &SpacingController::runningChanged, this, [this]() {
        if(m_spacingController->running())
            m_spacing->start();
    });
    connect(m_spacingController.get(), &SpacingController::finished, m_spacing, &QThread::quit);
    m_spacing->start();

    resetData();
}

MindMapController::~MindMapController()= default;

QAbstractItemModel* MindMapController::nodeModel() const
{
    return m_nodeModel.get();
}

QAbstractItemModel* MindMapController::linkModel() const
{
    return m_linkModel.get();
}

QString MindMapController::filename() const
{
    return m_filename;
}

void MindMapController::resetData()
{
    m_linkModel->clear();
    m_nodeModel->clear();

    auto root= new MindNode();
    root->setText(tr("Root"));
    m_nodeModel->appendNode(root);
}

void MindMapController::saveFile()
{
    QJsonObject data;
    QJsonArray nodeArray;
    auto nodes= m_nodeModel->nodes();
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
    auto links= m_linkModel->getDataSet();
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

    QFile file(m_filename);
    if(file.open(QFile::WriteOnly))
    {
        file.write(doc.toJson());
    }
}

void MindMapController::loadFile()
{
    QFile file(m_filename);
    if(!file.open(QFile::ReadOnly))
        qDebug() << "Error: file can't be read";

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

        m_nodeModel->appendNode(node);
        nodeMap.insert({node->id(), node});
    }

    for(auto linkRef : linkArray)
    {
        auto obj= linkRef.toObject();
        // auto link= new Link();

        auto idStart= obj["idStart"].toString();
        auto idEnd= obj["idEnd"].toString();

        auto link= m_linkModel->addLink(nodeMap[idStart], nodeMap[idEnd]);
        link->setVisible(obj["visible"].toBool());
        link->setDirection(static_cast<Link::Direction>(obj["Direction"].toInt()));
    }
}

void MindMapController::importFile(const QString& path)
{
    QFile file(QUrl(path).path());
    std::random_device rd;
    std::mt19937 gen(rd());

    if(!file.open(QFile::ReadOnly))
    {
        qDebug() << QStringLiteral("Error, file %1 can't be read.").arg(path);
        return;
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
        m_nodeModel->appendNode(node);

        if(newdepth == 1)
            qDebug() << newdepth << parent << text;
        if(newdepth > depth && previousNode != nullptr)
        {
            parent.append(previousNode);
        }
        if(newdepth < depth)
        {
            parent.removeLast();
            depth-= 1;
        }

        if(newdepth == 1)
            qDebug() << newdepth << parent << text;

        if(newdepth >= depth && !parent.isEmpty())
        {
            m_linkModel->addLink(parent.last(), node);
        }

        previousNode= node;
        depth= newdepth;
    }
}

void MindMapController::setFilename(const QString& path)
{
    if(path == m_filename)
        return;
    m_filename= path;
    emit filenameChanged();
}

void MindMapController::setSpacing(bool status)
{
    m_spacingController->setRunning(status);
}
bool MindMapController::spacing() const
{
    return m_spacingController->running();
}

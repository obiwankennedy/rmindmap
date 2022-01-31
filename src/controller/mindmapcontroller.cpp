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
#include "mindmapcontroller.h"

#include "command/additemcommand.h"
#include "command/removenodecommand.h"
#include "command/reparentingnodecommand.h"
#include "controller/selectioncontroller.h"
#include "controller/spacingcontroller.h"
#include "data/packagenode.h"
#include "model/minditemmodel.h"
#include "model/nodestylemodel.h"
#include "worker/fileserializer.h"

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>
#include <QUrl>
#include <random>

MindMapController::MindMapController(QObject* parent)
    : QObject(parent)
    , m_selectionController(new SelectionController())
    , m_itemModel(new MindItemModel())
    , m_styleModel(new NodeStyleModel())
{
    // m_itemModel->setLinkModel(m_linkModel.get());
    m_selectionController->setUndoStack(&m_stack);

    connect(&m_stack, &QUndoStack::canRedoChanged, this, &MindMapController::canRedoChanged);
    connect(&m_stack, &QUndoStack::canUndoChanged, this, &MindMapController::canUndoChanged);
    // connect(m_itemModel.get(), &MindItemModel::defaultStyleIndexChanged, this,
    // &MindMapController::defaultStyleIndexChanged); connect(m_itemModel.get(), &MindItemModel::nodeHeightChanged,
    // this, &MindMapController::contentRectChanged); connect(m_itemModel.get(), &MindItemModel::nodeWidthChanged, this,
    // &MindMapController::contentRectChanged);

    m_spacing= new QThread();
    m_spacingController.reset(new SpacingController(m_itemModel.get()));
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

MindMapController::~MindMapController()
{
    if(m_spacing->isRunning())
    {
        m_spacingController->setRunning(false);
        m_spacing->quit();
        m_spacing->wait();
        delete m_spacing;
    }

    auto model= m_itemModel.release();
    auto spacingCtrl= m_spacingController.release();

    delete spacingCtrl;
    delete model;
}

QAbstractItemModel* MindMapController::itemModel() const
{
    return m_itemModel.get();
}

QAbstractItemModel* MindMapController::styleModel() const
{
    return m_styleModel.get();
}

const QString& MindMapController::filename() const
{
    return m_filename;
}

const QString& MindMapController::errorMsg() const
{
    return m_errorMsg;
}

QRectF MindMapController::contentRect() const
{
    return m_itemModel->contentRect();
}

void MindMapController::clearData()
{
    // m_linkModel->clear();
    m_itemModel->clear();
}

void MindMapController::resetData()
{
    clearData();

    auto root= new MindNode();
    root->setText(tr("Root"));
    m_itemModel->appendItem(root);
}

void MindMapController::saveFile()
{
    if(!FileSerializer::writeFile(m_itemModel.get(), m_filename))
        setErrorMsg(tr("Error: File can't be loaded: %1").arg(m_filename));
}

void MindMapController::setErrorMsg(const QString& msg)
{
    if(m_errorMsg == msg)
        return;
    m_errorMsg= msg;
    emit errorMsgChanged();
}

void MindMapController::loadFile()
{
    clearData();
    if(!FileSerializer::readFile(m_itemModel.get(), m_filename))
        setErrorMsg(tr("Error: File can't be loaded: %1").arg(m_filename));
}

void MindMapController::importFile(const QString& path)
{
    clearData();
    if(!FileSerializer::readTextFile(m_itemModel.get(), path))
        setErrorMsg(tr("File can't be loaded: %1").arg(m_filename));
}

void MindMapController::setDefaultStyleIndex(int indx)
{
    if(indx == m_defaultStyleIndex)
        return;

    m_defaultStyleIndex= indx;
    emit defaultStyleIndexChanged();
}

void MindMapController::setFilename(const QString& path)
{
    if(path == m_filename)
        return;
    m_filename= QUrl(path).toLocalFile();
    if(!m_filename.endsWith(".rmap"))
        m_filename+= QStringLiteral(".rmap");
    emit filenameChanged();
}

NodeStyle* MindMapController::style(int index) const
{
    return m_styleModel->getStyle(index);
}

void MindMapController::setSpacing(bool status)
{
    m_spacingController->setRunning(status);
}

void MindMapController::redo()
{
    m_stack.redo();
}

void MindMapController::undo()
{
    m_stack.undo();
}

bool MindMapController::spacing() const
{
    return m_spacingController->running();
}

SelectionController* MindMapController::selectionController() const
{
    return m_selectionController.get();
}

bool MindMapController::canRedo() const
{
    return m_stack.canRedo();
}

void MindMapController::addBox(const QString& idparent)
{
    auto cmd= new AddItemCommand(m_itemModel.get(), MindItem::NodeType, this, idparent);
    m_stack.push(cmd);
}

void MindMapController::addPackage(const QPointF& pos)
{
    auto cmd= new AddItemCommand(m_itemModel.get(), MindItem::PackageType, this, {}, pos);
    m_stack.push(cmd);
}

void MindMapController::updatePackage(const QPointF& pos)
{
    if(!m_package)
        return;

    auto offset= pos - m_package->position();
    m_package->setWidth(offset.x());
    m_package->setHeight(offset.y());
}

void MindMapController::reparenting(MindNode* parent, const QString& id)
{
    auto cmd= new ReparentingNodeCommand(m_itemModel.get(), parent, id);
    m_stack.push(cmd);
}
void MindMapController::removeSelection()
{
    auto nodes= m_selectionController->selectedNodes();
    auto cmd= new RemoveNodeCommand(nodes, m_itemModel.get());
    m_stack.push(cmd);
}

void MindMapController::setCurrentPackage(PositionedItem* item)
{
    m_package= item;
}

bool MindMapController::canUndo() const
{
    return m_stack.canUndo();
}

int MindMapController::defaultStyleIndex() const
{
    return m_defaultStyleIndex;
}

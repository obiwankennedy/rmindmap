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

#include "command/addnodecommand.h"
#include "controller/selectioncontroller.h"
#include "controller/spacingcontroller.h"
#include "model/boxmodel.h"
#include "model/linkmodel.h"
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
    , m_linkModel(new LinkModel())
    , m_nodeModel(new BoxModel())
{
    m_nodeModel->setLinkModel(m_linkModel.get());
    m_selectionController->setUndoStack(&m_stack);

    connect(&m_stack, &QUndoStack::canRedoChanged, this, &MindMapController::canRedoChanged);
    connect(&m_stack, &QUndoStack::canUndoChanged, this, &MindMapController::canUndoChanged);

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

MindMapController::~MindMapController()
{
    if(m_spacing->isRunning())
    {
        m_spacingController->setRunning(false);
        m_spacing->quit();
        m_spacing->wait();
        delete m_spacing;
    }

    auto model= m_nodeModel.release();
    auto spacingCtrl= m_spacingController.release();

    delete spacingCtrl;
    delete model;
}

QAbstractItemModel* MindMapController::nodeModel() const
{
    return m_nodeModel.get();
}

QAbstractItemModel* MindMapController::linkModel() const
{
    return m_linkModel.get();
}

const QString& MindMapController::filename() const
{
    return m_filename;
}

const QString& MindMapController::errorMsg() const
{
    return m_errorMsg;
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
    if(!FileSerializer::writeFile(m_nodeModel.get(), m_linkModel.get(), m_filename))
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
    if(!FileSerializer::readFile(m_nodeModel.get(), m_linkModel.get(), m_filename))
        setErrorMsg(tr("Error: File can't be loaded: %1").arg(m_filename));
}

void MindMapController::importFile(const QString& path)
{
    if(!FileSerializer::readTextFile(m_nodeModel.get(), m_linkModel.get(), path))
        setErrorMsg(tr("File can't be loaded: %1").arg(m_filename));
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
    auto cmd= new AddNodeCommand(m_nodeModel.get(), m_linkModel.get(), idparent);
    m_stack.push(cmd);
}
void MindMapController::removeBox(const QString& id)
{
    auto cmd= new AddNodeCommand(m_nodeModel.get(), m_linkModel.get(), id);
    m_stack.push(cmd);
}

bool MindMapController::canUndo() const
{
    return m_stack.canUndo();
}

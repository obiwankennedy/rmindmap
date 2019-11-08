#include "selectioncontroller.h"

#include "command/dragnodecommand.h"
#include "data/mindnode.h"

SelectionController::SelectionController(QObject* parent) : QObject(parent) {}

void SelectionController::setEnabled(bool enable)
{
    if(enable == m_enabled)
        return;
    m_enabled= enable;
    emit enabledChanged();
}

void SelectionController::setUndoStack(QUndoStack* stack)
{
    m_undoStack= stack;
}

bool SelectionController::enabled() const
{
    return m_enabled;
}

void SelectionController::addToSelection(MindNode* node)
{
    if(node == nullptr)
        return;
    m_selection.push_back(node);
    node->setSelected(true);
    connect(node, &MindNode::itemDragged, this, &SelectionController::movingNode);
}

void SelectionController::removeFromSelection(MindNode* node)
{
    node->setSelected(false);
    disconnect(node, &MindNode::itemDragged, this, &SelectionController::movingNode);
    m_selection.erase(std::find(m_selection.begin(), m_selection.end(), node));
}

void SelectionController::clearSelection()
{
    std::for_each(m_selection.begin(), m_selection.end(), [this](MindNode* node) {
        node->setSelected(false);
        disconnect(node, &MindNode::itemDragged, this, &SelectionController::movingNode);
    });
    m_selection.clear();
}

void SelectionController::movingNode(const QPointF& motion)
{
    std::vector<QPointer<MindNode>> vec;
    std::transform(m_selection.begin(), m_selection.end(), std::back_inserter(vec),
                   [](MindNode* node) { return QPointer<MindNode>(node); });
    auto cmd= new DragNodeCommand(motion, vec);

    m_undoStack->push(cmd);
}

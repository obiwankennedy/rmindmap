#include "dragnodecommand.h"

#include "commandhelper.h"
#include "data/mindnode.h"

DragNodeCommand::DragNodeCommand(const QPointF& motion, const std::vector<QPointer<MindNode>>& selection)
    : m_motion(motion), m_mindNodes(selection)
{
}

void DragNodeCommand::undo()
{
    std::for_each(m_mindNodes.begin(), m_mindNodes.end(), [this](MindNode* node) { node->translate(m_motion * -1); });
}

void DragNodeCommand::redo()
{
    std::for_each(m_mindNodes.begin(), m_mindNodes.end(), [this](MindNode* node) {
        if(node->isDragged())
            return;
        node->translate(m_motion);
    });
}

int DragNodeCommand::id() const
{
    return static_cast<int>(RMindMap::CommandType::Drag);
}

bool DragNodeCommand::mergeWith(const QUndoCommand* other)
{
    if(other->id() != id())
        return false;
    auto command= dynamic_cast<const DragNodeCommand*>(other);

    if(nullptr == command)
        return false;

    if(command->getSelection() != getSelection())
        return false;

    m_motion+= command->getMotion();
    return true;
}

const QPointF& DragNodeCommand::getMotion() const
{
    return m_motion;
}
const std::vector<QPointer<MindNode>> DragNodeCommand::getSelection() const
{
    return m_mindNodes;
}

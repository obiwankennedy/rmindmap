#include "addnodecommand.h"

#include "model/boxmodel.h"

AddNodeCommand::AddNodeCommand(BoxModel* nodeModel, LinkModel* linkModel, const QString& idParent)
    : m_nodeModel(nodeModel), m_linkModel(linkModel), m_idParent(idParent)
{
}

void AddNodeCommand::undo()
{
    m_nodeModel->removeBox(m_mindNode);
    m_linkModel->removeLink(m_link);
}

void AddNodeCommand::redo()
{
    if(m_mindNode.isNull())
    {
        auto pair= m_nodeModel->addBox(m_idParent);
        m_mindNode= pair.first;
        m_link= pair.second;
    }
    else
    {
        m_nodeModel->appendNode(m_mindNode.data());
        m_linkModel->append(m_link);
    }
}

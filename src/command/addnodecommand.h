#ifndef ADDNODECOMMAND_H
#define ADDNODECOMMAND_H

#include <QPointer>
#include <QUndoCommand>

class MindNode;
class Link;
class BoxModel;
class LinkModel;
class AddNodeCommand : public QUndoCommand
{
public:
    AddNodeCommand(BoxModel* nodeModel, LinkModel* linkModel, const QString& idParent);
    void undo() override;
    void redo() override;

private:
    QPointer<MindNode> m_mindNode;
    QPointer<Link> m_link;
    BoxModel* m_nodeModel= nullptr;
    LinkModel* m_linkModel= nullptr;

    QString m_idParent;
};

#endif // ADDNODECOMMAND_H

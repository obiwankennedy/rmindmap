#ifndef REPARENTINGNODECOMMAND_H
#define REPARENTINGNODECOMMAND_H

#include <QUndoCommand>
#include <QPointer>
class MindNode;

class ReparentingNodeCommand : public QUndoCommand
{
public:    
   ReparentingNodeCommand():
   void undo() override;
   void redo() override;
private:
   QPointer<MindNode> m_mindNode;
   QPointer<MindNode> m_oldParent;
   QPointer<MindNode> m_newParent;

};


#endif // REPARENTINGNODECOMMAND_H

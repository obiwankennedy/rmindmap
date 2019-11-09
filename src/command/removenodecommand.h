#ifndef REMOVENODECOMMAND_H
#define REMOVENODECOMMAND_H

#include <QUndoCommand>
#include <QPointer>

class MindNode;
class RemoveNodeCommand : public QUndoCommand
{
public:    
   RemoveNodeCommand():
   void undo() override;
   void redo() override;
private:
   QPointer<MindNode> m_mindNode;

};


#endif // REMOVENODECOMMAND_H

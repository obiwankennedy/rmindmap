#ifndef DRAGNODECOMMAND_H
#define DRAGNODECOMMAND_H

#include <QPointF>
#include <QPointer>
#include <QUndoCommand>

class MindNode;

class DragNodeCommand : public QUndoCommand
{
public:
    DragNodeCommand(const QPointF& motion, const std::vector<QPointer<MindNode>>& selection);
    void undo() override;
    void redo() override;
    int id() const override;

    bool mergeWith(const QUndoCommand* other) override;

    const QPointF& getMotion() const;
    const std::vector<QPointer<MindNode>> getSelection() const;

private:
    QPointF m_motion;
    std::vector<QPointer<MindNode>> m_mindNodes;
};

#endif // DRAGNODECOMMAND_H

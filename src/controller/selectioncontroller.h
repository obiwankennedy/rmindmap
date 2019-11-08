#ifndef SELECTIONCONTROLLER_H
#define SELECTIONCONTROLLER_H

#include <QObject>
#include <QPointF>
#include <QUndoStack>

class MindNode;
class SelectionController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
public:
    explicit SelectionController(QObject* parent= nullptr);

    void setUndoStack(QUndoStack* stack);

    void setEnabled(bool enable);
    bool enabled() const;
signals:
    void enabledChanged();

public slots:
    void addToSelection(MindNode* node);
    void removeFromSelection(MindNode* node);
    void movingNode(const QPointF& motion);
    void clearSelection();

private:
    std::vector<MindNode*> m_selection;
    bool m_enabled= false;
    QUndoStack* m_undoStack;
};

#endif // SELECTIONCONTROLLER_H

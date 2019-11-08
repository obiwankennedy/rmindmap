#ifndef MINDMAPCONTROLLER_H
#define MINDMAPCONTROLLER_H

#include <QObject>
#include <QUndoStack>
#include <memory>

class QAbstractItemModel;
class BoxModel;
class LinkModel;
class SpacingController;
class SelectionController;

class MindMapController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QAbstractItemModel* nodeModel READ nodeModel CONSTANT)
    Q_PROPERTY(QAbstractItemModel* linkModel READ linkModel CONSTANT)
    Q_PROPERTY(QString filename READ filename WRITE setFilename NOTIFY filenameChanged)
    Q_PROPERTY(bool spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)
    Q_PROPERTY(SelectionController* selectionCtrl READ selectionController CONSTANT)
    Q_PROPERTY(bool canRedo READ canRedo NOTIFY canRedoChanged)
    Q_PROPERTY(bool canUndo READ canUndo NOTIFY canUndoChanged)
public:
    explicit MindMapController(QObject* parent= nullptr);
    ~MindMapController();

    QAbstractItemModel* nodeModel() const;
    QAbstractItemModel* linkModel() const;
    SelectionController* selectionController() const;
    QString filename() const;

    bool spacing() const;
    bool canRedo() const;
    bool canUndo() const;

signals:
    void filenameChanged();
    void spacingChanged();
    void canRedoChanged();
    void canUndoChanged();

public slots:
    void saveFile();
    void loadFile();
    void setFilename(const QString& path);
    void resetData();
    void setSpacing(bool b);
    void redo();
    void undo();
    void importFile(const QString& path);

private:
    QString m_filename;
    std::unique_ptr<LinkModel> m_linkModel;
    std::unique_ptr<BoxModel> m_nodeModel;
    std::unique_ptr<SpacingController> m_spacingController;
    std::unique_ptr<SelectionController> m_selectionController;
    QThread* m_spacing= nullptr;
    QUndoStack m_stack;
};

#endif // MINDMAPCONTROLLER_H

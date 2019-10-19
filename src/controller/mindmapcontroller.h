#ifndef MINDMAPCONTROLLER_H
#define MINDMAPCONTROLLER_H

#include <QObject>
#include <memory>

class QAbstractItemModel;
class BoxModel;
class LinkModel;
class SpacingController;

class MindMapController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QAbstractItemModel* nodeModel READ nodeModel CONSTANT)
    Q_PROPERTY(QAbstractItemModel* linkModel READ linkModel CONSTANT)
    Q_PROPERTY(QString filename READ filename WRITE setFilename NOTIFY filenameChanged)
    Q_PROPERTY(bool spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)
public:
    explicit MindMapController(QObject* parent= nullptr);
    ~MindMapController();

    QAbstractItemModel* nodeModel() const;
    QAbstractItemModel* linkModel() const;
    QString filename() const;

    bool spacing() const;

signals:
    void filenameChanged();
    void spacingChanged();

public slots:
    void saveFile();
    void loadFile();
    void setFilename(const QString& path);
    void resetData();
    void setSpacing(bool b);

private:
    QString m_filename;
    std::unique_ptr<LinkModel> m_linkModel;
    std::unique_ptr<BoxModel> m_nodeModel;
    std::unique_ptr<SpacingController> m_spacingController;
    QThread* m_spacing= nullptr;
};

#endif // MINDMAPCONTROLLER_H

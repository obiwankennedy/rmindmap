#ifndef SPACINGCONTROLLER_H
#define SPACINGCONTROLLER_H

#include <QObject>

class MindNode;
class LinkModel;
class Link;

class SpacingController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
public:
    explicit SpacingController(std::vector<MindNode*>& data, LinkModel* linkModel, QObject* parent= nullptr);

    bool running() const;
signals:
    void finished();
    void runningChanged();

public slots:
    void computeInLoop();
    void setRunning(bool run);

private:
    void applyCoulombsLaw(MindNode* node, std::vector<MindNode*> nodeList);
    void applyHookesLaw(Link* link);
    void attractToCenter();

private:
    std::vector<MindNode*>& m_data;
    LinkModel* m_linkModel;
    bool m_running= true;
};

#endif // SPACINGCONTROLLER_H

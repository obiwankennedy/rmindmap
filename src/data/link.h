#ifndef LINK_H
#define LINK_H

#include <QObject>
#include <QPointF>
#include <QPointer>

class MindNode;
class Link : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(Direction direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(QPointF startPoint READ startPoint NOTIFY startPointChanged)
    Q_PROPERTY(QPointF endPoint READ endPoint NOTIFY endPointChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
public:
    enum Direction
    {
        StartToEnd,
        EndToStart,
        Both
    };
    Q_ENUM(Direction)
    explicit Link(QObject* parent= nullptr);

    Direction direction() const;
    void setDirection(const Direction& direction);

    MindNode* start() const;
    void setStart(MindNode* start);

    MindNode* end() const;
    void setEnd(MindNode* end);

    QPointF endPoint() const;
    QPointF startPoint() const;

    float getLength() const;

    float getStiffness() const;
    void setStiffness(float stiffness);
    void setVisible(bool vi);
    bool isVisible() const;
    QString text() const;
signals:
    void linkChanged();
    void visibleChanged();
    void directionChanged();
    void startPointChanged();
    void endPointChanged();
    void textChanged();

public slots:
    void computePosition();
    void setText(const QString& text);

private:
    Direction m_dir= StartToEnd;
    bool m_visible= true;
    QPointer<MindNode> m_start;
    QPointer<MindNode> m_end;
    QString m_text;
    float m_stiffness= 400.0f;
};

#endif // LINK_H

#ifndef MINDNODE_H
#define MINDNODE_H

#include <QObject>
#include <QPointF>
#include <QVector2D>

class Link;
class MindNode : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QPointF position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QString imageUri READ imageUri WRITE setImageUri NOTIFY imageUriChanged)
    Q_PROPERTY(qreal contentWidth READ contentWidth WRITE setContentWidth NOTIFY contentWidthChanged)
    Q_PROPERTY(qreal contentHeight READ contentHeight WRITE setContentHeight NOTIFY contentHeightChanged)
    Q_PROPERTY(bool open READ open WRITE setOpen NOTIFY openChanged)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(bool hasLink READ hasLink NOTIFY hasLinkChanged)
public:
    MindNode(QObject* parent= nullptr);
    virtual ~MindNode();

    MindNode* parentNode() const;
    void setParentNode(MindNode* parent);

    void setNextPosition(const QPointF& pos, Link* emiter);

    QPointF position() const;
    void setPosition(const QPointF& pos);

    QVector2D getVelocity() const;
    void setVelocity(const QVector2D& velocity);

    QVector2D getAcceleration() const;
    void setAcceleration(const QVector2D& acceleration);
    void applyForce(const QVector2D& force);

    QString text() const;
    int getMass() const;

    QString id() const;
    qreal contentWidth() const;
    void setContentWidth(qreal w);
    qreal contentHeight() const;
    void setContentHeight(qreal h);

    QPointF centerPoint() const;
    QRectF boundingRect() const;

    bool open() const;
    void setOpen(bool op);

    bool isVisible() const;
    void setVisible(bool op);

    void addLink(Link* link);
    bool hasLink() const;

    QString imageUri() const;
    void setId(const QString& id);

public slots:
    void setText(QString text);
    void setMass(int mass);
    void setImageUri(const QString& uri);

signals:
    void positionChanged(const QPointF oldPos);
    void textChanged(QString text);
    void contentWidthChanged();
    void contentHeightChanged();
    void openChanged();
    void visibleChanged();
    void hasLinkChanged();
    void imageUriChanged();
    void idChanged();

protected:
    void updatePosition();
    void computeContentSize();

    void setLinkVisibility();

private:
    MindNode* m_parent= nullptr;
    std::vector<QPointer<Link>> m_subNodelinks;
    QPointF m_position;
    QString m_id;
    QString m_text;
    bool m_open= true;
    bool m_visible= true;
    QString m_imageUri;

    qreal m_width= 100;
    qreal m_height= 20;

    int m_mass= 1;
    std::map<Link*, QPointF> m_nextPositions;
    QVector2D _velocity;
    QVector2D _acceleration;
};

#endif // MINDNODE_H

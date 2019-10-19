#ifndef LINK_H
#define LINK_H

#include <QObject>

class LInk : public QObject
{
    Q_OBJECT
public:
    explicit LInk(QObject *parent = nullptr);

signals:

public slots:
};

#endif // LINK_H

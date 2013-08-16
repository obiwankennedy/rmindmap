#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <QSettings>


class SettingsUser
{
public:
    virtual void readSettings(QSettings&) = 0;
    virtual void writeSettings(QSettings&) = 0;
};

class Serialisable
{
public:
    virtual void readFromData(QDataStream&) = 0;
    virtual void writeToData(QDataStream&) = 0;
};

#endif // SERIALIZABLE_H

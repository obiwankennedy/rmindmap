#ifndef STRINGMANAGER_H
#define STRINGMANAGER_H

#include <QObject>
#include <QList>
#include <QMap>

#include "src/serializable.h"
/**
 * @brief The StringManager class - provides features to translate a mindmap dynamically.
 */
class StringManager : public QObject, public Serialisable
{
    Q_OBJECT
public:
    explicit StringManager(QObject *parent = 0);

    void setValue(QString key, QString value);

    QString getValue(QString key);

    void setCurrentLang(int);

    virtual void readFromData(QDataStream&);
    virtual void writeToData(QDataStream&);


private:
    QMap<QString,QString> m_currentLang;
    QList<QMap<QString,QString> > m_availableLang;


    
};

#endif // STRINGMANAGER_H

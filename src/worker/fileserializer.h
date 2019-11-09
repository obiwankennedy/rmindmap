#ifndef FILESERIALIZER_H
#define FILESERIALIZER_H

#include <QObject>
class BoxModel;
class LinkModel;
class FileSerializer : public QObject
{
    Q_OBJECT
public:
    FileSerializer();

    static bool readTextFile(BoxModel* nodeModel, LinkModel* linkModel, const QString& filepath);
    static bool readFile(BoxModel* nodeModel, LinkModel* linkModel, const QString& filepath);
    static bool writeFile(BoxModel* nodeModel, LinkModel* linkModel, const QString& filepath);
};

#endif // FILESERIALIZER_H

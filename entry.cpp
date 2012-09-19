#include "entry.h"

Entry::Entry(QString fileName, QString fileData, QDateTime modificationDate, QObject *parent) :
    QObject(parent),
    fileName(fileName),
    fileData(fileData),
    modificationDate(modificationDate)
{
}

QString Entry::getFileData()
{
    return fileData;
}

QString Entry::getFileName()
{
    return fileName;
}

QDateTime Entry::getModificationDate()
{
    return modificationDate;
}

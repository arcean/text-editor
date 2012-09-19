#ifndef ENTRY_H
#define ENTRY_H

#include <QObject>
#include <QString>
#include <QDateTime>

class Entry : public QObject
{
    Q_OBJECT
public:
    explicit Entry(QString fileName, QString fileData, QDateTime modificationDate, QObject *parent = 0);

    QString getFileName();
    QString getFileData();
    QDateTime getModificationDate();
    
signals:
    
public slots:

private:
    QString fileName;
    QString fileData;
    QDateTime modificationDate;
    
};

#endif // ENTRY_H

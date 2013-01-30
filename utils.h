#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QString>
#include <QDateTime>

class Utils : public QObject
{
    Q_OBJECT
public:
    explicit Utils(QObject *parent = 0);

    QString getTextFromFile(int position);
    QDateTime getModificationDate(int position);
    QString getFileName(int position);
    QString getNewFilenameWithDate();
    QString getNewFilename(QString text);
    
signals:
    
public slots:
    
};

#endif // UTILS_H

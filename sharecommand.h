#ifndef SHARECOMMAND_H
#define SHARECOMMAND_H

#include <QObject>

class ShareCommand : public QObject
{
    Q_OBJECT
public:
    explicit ShareCommand(QObject *parent = 0);

    void share(QString filePath);
    
signals:
    
public slots:

private:

};

#endif // SHARECOMMAND_H

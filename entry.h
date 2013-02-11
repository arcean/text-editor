/***************************************************************************
**
** Copyright (C) 2012, 2013 Tomasz Pieniążek
** All rights reserved.
** Contact: Tomasz Pieniążek <t.pieniazek@gazeta.pl>
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifndef ENTRY_H
#define ENTRY_H

#include <QObject>
#include <QString>
#include <QDateTime>

class Entry : public QObject
{
    Q_OBJECT
public:
    explicit Entry(QString fileName, QString fileData, const QDateTime &modificationDate, int id, QObject *parent = 0);

    QString getFileName();
    QString getFileData();
    QDateTime getModificationDate();
    int getId();

    void setFileData(QString fileData);
    void setFileName(QString fileName);
    void setModificationDate(const QDateTime &modificationDate);
    void setId(int id);
    
signals:
    
public slots:

private:
    QString fileName;
    QString fileData;
    QDateTime modificationDate;
    int id;
    
};

#endif // ENTRY_H

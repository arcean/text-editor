/***************************************************************************
**
** Copyright (C) 2013 Tomasz Pieniążek
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

    static QString getTextFromFile(int position);
    static QDateTime getModificationDate(int position);
    static QString getFileName(int position);
    static QString getNewFilenameWithDate();
    static void getNewFilename(QString &text);
    static bool createDirIfNotExists();
    static QString readDataToTxt(const QString& filePath);
    
signals:
    
public slots:
    
};

#endif // UTILS_H

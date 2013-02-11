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

#include "entry.h"

Entry::Entry(QString fileName, QString fileData, const QDateTime &modificationDate, int id, QObject *parent) :
    QObject(parent),
    fileName(fileName),
    fileData(fileData),
    modificationDate(modificationDate),
    id(id)
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

int Entry::getId()
{
    return id;
}

void Entry::setFileData(QString fileData)
{
    this->fileData = fileData;
}

void Entry::setFileName(QString fileName)
{
    this->fileName = fileName;
}

void Entry::setModificationDate(const QDateTime &modificationDate)
{
    this->modificationDate = modificationDate;
}

void Entry::setId(int id)
{
    this->id = id;
}

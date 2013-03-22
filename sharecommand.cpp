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

#include <QString>
#include <MDataUri>
#include <QDebug>
#include <maemo-meegotouch-interfaces/shareuiinterface.h>
#include <QFile>

#include "sharecommand.h"

ShareCommand::ShareCommand(QObject *parent) :
    QObject(parent)
{
}

void ShareCommand::share(const QString &filePath)
{
    QString targetName;

    if (filePath.at(0) == QChar('f')) {
        targetName = filePath;
        targetName.replace(0,7,"");
    }
    else
        targetName = filePath;

    QFileInfo shareFileInfo(targetName);
    QString shareFileURL = shareFileInfo.canonicalFilePath();

    if (shareFileURL.isEmpty()) {
        qCritical() << "Empty sharing file URL...";
        return;
    }

    QStringList items;
    items << shareFileURL;

    ShareUiInterface shareIf("com.nokia.ShareUi");

    if (shareIf.isValid()) {
        shareIf.share(items);
    }
    else
        qCritical() << "Invalid ShareUi";

}

void ShareCommand::shareAsText(const QString &text, const QString &title)
{
    MDataUri duri;

    duri.setMimeType ("text/x-url");
    duri.setTextData (text, "utf-8");

   // if (!title.isEmpty())
  //      duri.setAttribute ("title", title);

    qDebug() << "T1" << text;
    qDebug() << "T2" << duri.textData();

    //duri.setAttribute ("description", desc);

    if (!duri.isValid()) {
        qCritical() << "Invalid URI";
        return;
    }

    if (text.isEmpty()) {
        qCritical() << "Empty sharing text...";
        return;
    }

    QStringList items;
    items << duri.toString();

    ShareUiInterface shareIf("com.nokia.ShareUi");

    if (shareIf.isValid())
        shareIf.share (items);
    else {
        qCritical() << "Invalid interface";
        return;
    }
}

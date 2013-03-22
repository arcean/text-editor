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

#include <QFile>
#include <QDir>
#include <QRegExp>
#include <QDebug>

#include "utils.h"

Utils::Utils(QObject *parent) :
    QObject(parent)
{
}

bool Utils::createDirIfNotExists()
{
    QDir dir("/home/user/MyDocs/exnote");

    if (!dir.exists())
        return dir.mkpath(".");

    return false;
}

QDateTime Utils::getModificationDate(int position)
{
    QDir files("/home/user/MyDocs/exnote/");
    QFileInfoList fileList = files.entryInfoList(QDir::Files, QDir::Time);
    int fileListLength = fileList.length();

    if (position >= fileListLength || position < 0) {
        qWarning() << "[E]: No such file:" << position;
        return QDateTime();
    }

    QFileInfo fileInfo (fileList.at(position).filePath());

    return fileInfo.lastModified();
}

QString Utils::getFileName(int position)
{
    QDir files("/home/user/MyDocs/exnote/");
    QFileInfoList fileList = files.entryInfoList(QDir::Files, QDir::Time);
    int fileListLength = fileList.length();

    if (position >= fileListLength || position < 0) {
        qWarning() << "[E]: No such file:" << position;
        return "";
    }

    return fileList.at(position).filePath();
}

QString Utils::getTextFromFile(int position)
{
    QDir files("/home/user/MyDocs/exnote/");
    QFileInfoList fileList = files.entryInfoList(QDir::Files, QDir::Time);
    int fileListLength = fileList.length();

    if (position >= fileListLength || position < 0) {
        qWarning() << "[E]: No such file:" << position;
        return "";
    }

    QFile file (fileList.at(position).filePath());

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "";

    QTextStream in(&file);
    QString line = in.readLine();

    while (!in.atEnd()) {
        line += in.readLine();

        if (line.length() > 1200) {
            break;
        }
    }

    //// Parse the title
    line.replace(QRegExp("<[^>]*/p>"), QString('\n'));
    line.remove(QRegExp("<[^>]*>"));
    line.remove("p, li { white-space: pre-wrap; }", Qt::CaseSensitive);
    // Get only the first line from the title
    QStringList titleList = line.split(QString('\n'));

    for (int i = 0; i < titleList.length(); i++) {
        QString newTitle = titleList[i];
        newTitle.remove(QString('\n'));

        if (!newTitle.isEmpty()) {
            line = newTitle;
            break;
        }
    }

    // Check if first char is an empty space
    while (line.at(0) == ' ' || line.at(0) == '\n') {
        line.remove(0, 1);
    }

    return line;
}

QString Utils::getNewFilenameWithDate()
{
    /* Create the app directory */
    createDirIfNotExists();

    QDir appDir("/home/user/MyDocs/exnote/");
    bool ready = false;
    int counter = 0;
    QDateTime time = QDateTime::currentDateTime();
    QString baseName = time.toString("dd_mm_yy");

    if(!appDir.exists()) {
        appDir.mkdir("/home/user/MyDocs/exnote/");
    }

    QDir::setCurrent("/home/user/MyDocs/exnote/");
    QFile file;

    while (!ready) {
        if(counter > 0)
            file.setFileName(baseName + "(" + QString::number(counter) +").html");
        else
            file.setFileName(baseName + ".html");

        if(!file.exists()) {
            return file.fileName();
        }

        counter++;

        if(counter > 500000)
            ready = true;
    }

    return file.fileName();
}

void Utils::getNewFilename(QString &text)
{
    /* Create the app directory */
    createDirIfNotExists();

    /* Max filename length ~= 28 */
    if (text.length() > 28)
        text.chop(text.length() - 28);

    /* Parse the title */
    text.replace(QRegExp("<[^>]*/p>"), QString('\n'));
    text.remove(QRegExp("<[^>]*>"));
    text.remove("p, li { white-space: pre-wrap; }", Qt::CaseSensitive);
    /* Replace forbidden chars */
    text.replace(".", "_");
    text.replace(",", "_");
    text.replace(";", "_");
    text.replace(":", "_");
    text.replace("#", "_");
    text.replace("%", "_");
    text.replace("&", "_");
    text.replace("*", "_");
    text.replace("<", "_");
    text.replace(">", "_");
    text.replace("?", "_");
    text.replace("/", "_");
    text.replace("|", "_");
    text.replace("\\", "_");
    text.replace("{", "_");
    text.replace("}", "_");
    text.replace("\"", "_");
    text.replace("'", "_");
    // Get only the first line from the title
    QStringList titleList = text.split(QString('\n'));

    for (int i = 0; i < titleList.length(); i++) {
        QString newTitle = titleList[i];
        newTitle.remove(QString('\n'));

        if (!newTitle.isEmpty()) {
            text = newTitle;
            break;
        }
    }

    // Check if first char is an empty space
    while (text.at(0) == ' ' || text.at(0) == '\n') {
        text.remove(0, 1);
    }

    text.replace(" ", "_");

    QDir::setCurrent("/home/user/MyDocs/exnote/");
    QFile file;
    bool ready = false;
    int counter = 0;

    while (!ready) {
        if(counter > 0)
            file.setFileName(text + "(" + QString::number(counter) +").html");
        else
            file.setFileName(text + ".html");

        if(!file.exists()) {
            text = file.fileName();
            return;
        }

        counter++;

        if(counter > 65536)
            ready = true;
    }

    text = file.fileName();
}

QString Utils::readDataToTxt(const QString &filePath)
{
    QFile file (filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "[E]: could not open the file" << filePath;
        return "";
    }

    QTextStream in(&file);
    QString line = in.readLine();

    while (!in.atEnd()) {
        line += in.readLine();
    }

    //// Parse the title
    line.replace(QRegExp("<[^>]*/p>"), QString('\n'));
    line.remove(QRegExp("<[^>]*>"));
    line.remove("p, li { white-space: pre-wrap; }", Qt::CaseSensitive);

    qDebug() << "Ret" << line;

    return line;
}

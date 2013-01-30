#include <QFile>
#include <QDir>
#include <QRegExp>
#include <QDebug>

#include "utils.h"

Utils::Utils(QObject *parent) :
    QObject(parent)
{
}

QDateTime Utils::getModificationDate(int position)
{
    QDir files("/home/user/MyDocs/exnote/");
    QFileInfoList fileList = files.entryInfoList(QDir::Files, QDir::Time);
    int fileListLength = fileList.length();

    if (position >= fileListLength || position < 0) {
        qDebug() << "[E]: No such file:" << position;
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
        qDebug() << "[E]: No such file:" << position;
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
        qDebug() << "[E]: No such file:" << position;
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

QString Utils::getNewFilename(QString text)
{
    //// Parse the title
    text.replace(QRegExp("<[^>]*/p>"), QString('\n'));
    text.remove(QRegExp("<[^>]*>"));
    text.remove("p, li { white-space: pre-wrap; }", Qt::CaseSensitive);
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
            qDebug() << "Rerutns" << file.fileName();
            return file.fileName();
        }

        counter++;

        if(counter > 500000)
            ready = true;
    }


    qDebug() << "Rerutns" << file.fileName();

    return file.fileName();
}

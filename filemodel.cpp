#include <QDir>
#include <QFileInfoList>
#include <QDebug>
#include "filemodel.h"

FileModel::FileModel(QObject *parent)
   : MAbstractItemModel(parent)
{
    numFiles = countFiles();

    for (int i = 0; i < numFiles; i++) {
        filesData.append(getTextFromFile(i));
        lastModification.append(getModificationDate(i));
    }
}

void FileModel::reload()
{
    numFiles = countFiles();
    qDebug() << "numFiles" << numFiles;
    filesData.clear();

    for (int i = 0; i < numFiles; i++) {
        filesData.append(getTextFromFile(i));
    }

    for (int i = 0; i < numFiles; i++)
        emit dataChanged(createIndex(i, 0), createIndex(i, 0));
}

int FileModel::countFiles()
{
    QDir files("/home/user/MyDocs/exnote/");
    QFileInfoList fileList = files.entryInfoList(QDir::Files, QDir::Time);

    return fileList.length();
}

QDateTime FileModel::getModificationDate(int position)
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

QString FileModel::getTextFromFile(int position)
{
    QDir files("/home/user/MyDocs/exnote/");
    QFileInfoList fileList = files.entryInfoList(QDir::Files, QDir::Time);
    int fileListLength = fileList.length();

    if (position >= fileListLength || position < 0) {
        qDebug() << "[E]: No such file:" << position;
        return "";
    }

    QFile file (fileList.at(position).filePath());
    qDebug() << "FN:" << fileList.at(position).filePath();

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
    if (line.at(0) == ' ' || line.at(0) == '\n') {
        line.remove(0, 1);
    }

    return line;
}

int FileModel::rowCount(const QModelIndex &parent) const
{
   Q_UNUSED(parent);
   return filesData.size();
}
/*
QVariant FileModel::data(const QModelIndex &index, int role) const
{
  if (role == Qt::DisplayRole) {
      QStringList rowData;
      rowData << filesData.at(index.row());

      return QVariant(rowData);
  }

  return QVariant();
}*/

int FileModel::groupCount() const
{
    return 0;
}

int FileModel::rowCountInGroup(int group) const
{
    return filesData.count();
}

QString FileModel::groupTitle(int group) const
{
    return "";
}

QVariant FileModel::itemData(int row, int group, int role) const
{
    int flatRow = row;
    Q_ASSERT(flatRow >= 0);
    Q_ASSERT(flatRow < filesData.size());

    if (role == Qt::DisplayRole) {
        QStringList rowData;
        qDebug() << "index:" << row;
        rowData << filesData.at(row);
        // Convert QDateTime to QString e.g. 17 may 2012
        rowData << lastModification.at(row).toString("dd MMMM yyyy");
        return QVariant(rowData);
    }

    return QVariant();

}

bool FileModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (count <= 0)
        return true; // Successfully added 0 rows.

    QList<QString> data = filesData;
    QList<QDateTime> data2 = lastModification;
    QString entry;
    QDateTime modification;

    for (int i = row; i < row + count; i++) {
        entry = getTextFromFile(i);
        modification = getModificationDate(i);

        if (!entry.isEmpty())
            data.insert(i, entry);

        data2.insert(i, modification);
    }  

    QModelIndex realParent = parent;
    beginInsertRows(realParent, row, row + count - 1, count == 1);
    filesData = data;
    lastModification = data2;
    endInsertRows();

    return true;

}

bool FileModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (count <= 0)
        return true; //Successfully removed 0 rows.

    Q_ASSERT(row >= 0);
    Q_ASSERT(row < filesData.size());
    Q_ASSERT(row < lastModification.size());

    beginRemoveRows(parent, row, row + count - 1, count == 1);
    //qDeleteAll(filesData.begin() + flatRow, filesData.begin() + flatRow + count - 1);
    filesData.removeAt(row);
    lastModification.removeAt(row);
    endRemoveRows();

    return true;
}

QString FileModel::getFilePath(int index)
{
    QDir files("/home/user/MyDocs/exnote/");
    QFileInfoList fileList = files.entryInfoList(QDir::Files, QDir::Time);
    int fileListLength = fileList.length();

    if (index >= fileListLength || index < 0) {
        qDebug() << "[E]: No such file:" << index;
        return "";
    }

    return fileList.at(index).filePath();
}

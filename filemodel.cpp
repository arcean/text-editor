#include <QDir>
#include <QFileInfoList>
#include <QDebug>
#include "filemodel.h"

FileModel::FileModel(QObject *parent)
   : MAbstractItemModel(parent),
     entryList(),
     buckets()
{
    numFiles = countFiles();
    setGrouped(true);

   /* for (int i = 0; i < numFiles; i++) {
        filesData.append(getTextFromFile(i));
        lastModification.append(getModificationDate(i));
    }*/
    insertRows(0, numFiles, QModelIndex());
}

FileModel::~FileModel()
{
    qDeleteAll(entryList);
}
/*
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
}*/

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

QString FileModel::getFileName(int position)
{
    QDir files("/home/user/MyDocs/exnote/");
    QFileInfoList fileList = files.entryInfoList(QDir::Files, QDir::Time);
    int fileListLength = fileList.length();

    if (position >= fileListLength || position < 0) {
        qDebug() << "[E]: No such file:" << position;
        return "";
    }
    qDebug() << "FilesName:" << fileList.at(position).filePath();
    return fileList.at(position).filePath();
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

/*int FileModel::rowCount(const QModelIndex &parent) const
{
   Q_UNUSED(parent);
   return filesData.count();
}*/
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
    return buckets.bucketCount();
}

int FileModel::rowCountInGroup(int group) const
{
    if (group == -1) {
        return entryList.count();
    } else {
        return buckets.bucketSize(group);
    }
}

QString FileModel::groupTitle(int group) const
{
    return buckets.bucketName(group);
}

QVariant FileModel::itemData(int row, int group, int role) const
{
    int flatRow = row;

    if (group >= 0 && row >= 0)
        flatRow = buckets.origItemIndex(group, row);

    Q_ASSERT(flatRow >= 0);
    Q_ASSERT(flatRow < filesData.size());

    if (role == Qt::DisplayRole) {
        QStringList rowData;
        qDebug() << "index:" << row << " flatrow" << flatRow;
        rowData << entryList[flatRow]->getFileData();
        // Convert QDateTime to QString e.g. 17 may 2012
        rowData << entryList[flatRow]->getModificationDate().toString("dd MMMM yyyy");
        return QVariant(rowData);
    }
    else if (role == EntrySortRole)
        return QVariant::fromValue(entryList[flatRow]->getFileData());
    else if (role == EntryFilterRole)
        return QVariant::fromValue(entryList[flatRow]->getFileData());

    return QVariant();

}

bool FileModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (count <= 0)
        return true; // Successfully added 0 rows.

    QVector<Entry *> entries = entryList;
    Entry *entry = NULL;
    QString data;
    QString name;
    QDateTime modification;

    for (int i = row; i < row + count; i++) {
        data = getTextFromFile(i);
        name = getFileName(i);
        modification = getModificationDate(i);

        entry = new Entry(name, data, modification);

        if (entry)
            entries.insert(i, entry);
    }

    QStringList entriesList;
    foreach (Entry *entry, entries) {
        Q_ASSERT(entry);
        entriesList << entry->getFileData();
    }

    MLocaleBuckets entryBuckets(entriesList);
    if (entryBuckets.isEmpty()) {
        qDeleteAll(entries.begin() + entryList.size(), entries.end());
        return false; // Something went wrong.
    }

    QModelIndex realParent = parent;
    if (entry) {
        if (isGrouped()) {
            for (int i = 0; i < entryBuckets.bucketCount(); i++) {
                for (int j = 0; j < entryBuckets.bucketSize(i); j++) {
                    if (entryBuckets.bucketContent(i).at(j) == entry->getFileData()) {
                        row = j;
                        realParent = index(i, 0, QModelIndex());
                        break;
                    }
                }
            }
        }
    }
    if (isGrouped() && !parent.isValid())
        beginInsertRows(QModelIndex(), row, row + (entryBuckets.bucketCount() - buckets.bucketCount()) - 1, count == 1);
    else
        beginInsertRows(realParent, row, row + count - 1, count == 1);

    entryList = entries;
    buckets = entryBuckets;

    endInsertRows();

    if (isGrouped()) {
        layoutAboutToBeChanged();
        layoutChanged();
    }

    return true;
}

bool FileModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (count <= 0)
        return true; //Successfully removed 0 rows

    int flatRow = row;
    int group = parent.row();

    if (isGrouped() && group >= 0)
        flatRow = buckets.origItemIndex(group, row);

    Q_ASSERT(row >= 0);
    Q_ASSERT(row < filesData.size());
    Q_ASSERT(row < lastModification.size());

    beginRemoveRows(parent, row, row + count - 1, count == 1);
    qDeleteAll(entryList.begin() + flatRow, entryList.begin() + flatRow + count - 1);
    entryList.remove(flatRow, count);

    if (isGrouped() && group >= 0) {
        buckets.removeBucketItems(group, row, count);
        buckets.removeEmptyBucket(group);
    }
    else
        regenerateModel();

    endRemoveRows();

    return true;
}

void FileModel::clear()
{
    if (!entryList.isEmpty()) {
        beginResetModel();
        qDeleteAll(entryList);
        entryList.clear();
        buckets.clear();
        endResetModel();
    }
}

void FileModel::regenerateModel()
{
    QStringList itemList;

    foreach (Entry *entry, entryList) {
        Q_ASSERT(entry);
        itemList << entry->getFileData();
    }

    buckets.setItems(itemList);
}

void FileModel::updateData(const QModelIndex &first, const QModelIndex &last)
{
    emit dataChanged(first, last);
}

QString FileModel::getFilePath(int index, int parentIndex)
{
    // Where: parentIndex == parent.row() == group
    if (isGrouped() && parentIndex >= 0)
        index = buckets.origItemIndex(parentIndex, index);

    if (index < 0 || index > entryList.size()) {
        qDebug() << "[E]: index >> filesName";
        return "";
    }
    qDebug() << "index" << index << "FN" << entryList[index]->getFileName();

    return entryList[index]->getFileName();
}

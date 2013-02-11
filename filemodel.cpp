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

#include <QDir>
#include <QFileInfoList>
#include <QDebug>

#include "filemodel.h"

FileModel::FileModel(QObject *parent)
   : MAbstractItemModel(parent),
     numFiles(-1),
     entryList(),
     buckets(),
     dateBuckets()
{
    // Load sorting settings
    selectedBuckets = Settings::getSortMode();

    numFiles = countFiles();
    insertRows(0, numFiles, QModelIndex());
}

FileModel::~FileModel()
{
    qDeleteAll(entryList);
}

int FileModel::countFiles()
{
    QDir files("/home/user/MyDocs/exnote/");
    QFileInfoList fileList = files.entryInfoList(QDir::Files, QDir::Time);

    return fileList.length();
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
    if (selectedBuckets)
        return buckets.bucketCount();
    else
        return dateBuckets.bucketCount();
}

int FileModel::rowCountInGroup(int group) const
{
    if (group == -1) {
        return entryList.count();
    } else {
        if (selectedBuckets)
            return buckets.bucketSize(group);
        else
            return dateBuckets.bucketSize(group);
    }
}

QString FileModel::groupTitle(int group) const
{
    if (selectedBuckets)
        return buckets.bucketName(group);
    else
        return dateBuckets.bucketName(group);
}

QVariant FileModel::itemData(int row, int group, int role) const
{
    int flatRow = row;

    if (group >= 0 && row >= 0) {
        if (selectedBuckets)
            flatRow = buckets.origItemIndex(group, row);
        else
            flatRow = dateBuckets.origItemIndex(group, row);
    }

    Q_ASSERT(flatRow >= 0);
    Q_ASSERT(flatRow < entryList.size());

    if (role == Qt::DisplayRole) {
        QStringList rowData;

        rowData << entryList[flatRow]->getFileData();
        // Convert QDateTime to QString e.g. 17 may 2012
        rowData << entryList[flatRow]->getModificationDate().toString("dd MMMM yyyy");
        // Required by storing system
        rowData << entryList[flatRow]->getFileName();
        rowData << QString::number(entryList[flatRow]->getId());
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
        data = Utils::getTextFromFile(i);
        name = Utils::getFileName(i);
        modification = Utils::getModificationDate(i);

        entry = new Entry(name, data, modification, i);

        if (entry)
            entries.insert(i, entry);
    }

    QStringList entriesList;
    foreach (Entry *entry, entries) {
        Q_ASSERT(entry);
        if (selectedBuckets)
            entriesList << entry->getFileData();
        else
            entriesList << entry->getModificationDate().toString("dd MMMM yyyy");
    }

    if (selectedBuckets) {
        // MLocaleBuckets
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

    }
    else {

        // MBuckets
        MBuckets entryBuckets(entriesList);
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
            beginInsertRows(QModelIndex(), row, row + (entryBuckets.bucketCount() - dateBuckets.bucketCount()) - 1, count == 1);
        else
            beginInsertRows(realParent, row, row + count - 1, count == 1);

        entryList = entries;
        dateBuckets = entryBuckets;
    }

    regenerateEntries();
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

    if (isGrouped() && group >= 0) {
        if (selectedBuckets)
            flatRow = buckets.origItemIndex(group, row);
        else
            flatRow = dateBuckets.origItemIndex(group, row);
    }

    Q_ASSERT(row >= 0);
    Q_ASSERT(row < entryList.size());

    beginRemoveRows(parent, row, row + count - 1, count == 1);
    qDeleteAll(entryList.begin() + flatRow, entryList.begin() + flatRow + count - 1);
    entryList.remove(flatRow, count);

    if (isGrouped() && group >= 0) {
        if (selectedBuckets)
            buckets.removeBucketItems(group, row, count);
        else
            dateBuckets.removeBucketItems(group, row, count);
    }

    regenerateModel();
    regenerateEntries();

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
        dateBuckets.clear();
        endResetModel();
    }
}

void FileModel::regenerateModel()
{
    QStringList itemList;

    foreach (Entry *entry, entryList) {
        Q_ASSERT(entry);
        if (selectedBuckets)
            itemList << entry->getFileData();
        else
            itemList << entry->getModificationDate().toString("dd MMMM yyyy");
    }

    if (selectedBuckets)
        buckets.setItems(itemList);
    else
        dateBuckets.setItems(itemList);
}

void FileModel::changeModel()
{
    QStringList itemList;

    // Update selectedBuckets value, which could've been changed
    selectedBuckets = Settings::getSortMode();

    foreach (Entry *entry, entryList) {
        Q_ASSERT(entry);
        if (selectedBuckets)
            itemList << entry->getFileData();
        else
            itemList << entry->getModificationDate().toString("dd MMMM yyyy");
    }

    beginResetModel();
    buckets.clear();
    dateBuckets.clear();

    if (selectedBuckets)
        buckets.setItems(itemList);
    else
        dateBuckets.setItems(itemList);

    endResetModel();
}

void FileModel::updateData(const QModelIndex &first, const QModelIndex &last)
{
    emit dataChanged(first, last);
}

QString FileModel::getFilePath(int index, int parentIndex)
{
    // Where: parentIndex == parent.row() == group
    if (isGrouped() && parentIndex >= 0) {
        if (selectedBuckets)
            index = buckets.origItemIndex(parentIndex, index);
        else
            index = dateBuckets.origItemIndex(parentIndex, index);
    }

    if (index < 0 || index > entryList.size()) {
        qWarning() << "[E]: index >> filesName";
        return "";
    }

    return entryList[index]->getFileName();
}

int FileModel::getCurrentRow(int row, int parentRow)
{
    if (isGrouped() && parentRow >= 0) {
        if (selectedBuckets)
            row = buckets.origItemIndex(parentRow, row);
        else
            row = dateBuckets.origItemIndex(parentRow, row);
    }

    return row;
}

int FileModel::getNumOfEntries()
{
    return entryList.size();
}

void FileModel::regenerateEntries()
{
    if (entryList.size() > 1) {
        for (int i = 1; i < entryList.size(); i++) {
            entryList[i]->setId(i);
        }
    }
}

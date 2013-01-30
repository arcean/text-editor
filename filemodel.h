#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <MAbstractItemModel>
#include <QDateTime>
#include <MLocaleBuckets>
#include <QVector>

#include "entry.h"
#include "utils.h"
#include "singleton.h"

class FileModel : public MAbstractItemModel
{
    Q_OBJECT

 public:
    // Defining roles here which will be used for sorting and filtering in FileSortedModel
    enum EntryRoles {
        EntrySortRole = Qt::UserRole + 1,
        EntryFilterRole
    };

    FileModel(QObject *parent = 0);
    virtual ~FileModel();

   // int rowCount(const QModelIndex &parent) const;
   // QVariant data(const QModelIndex &index, int role) const;
    bool removeRows(int row, int count, const QModelIndex &parent);
    bool insertRows(int row, int count, const QModelIndex &parent);
    QVariant itemData(int row, int group, int role) const;
    int rowCountInGroup(int group) const;
    QString groupTitle(int group) const;
    int groupCount() const;
    void updateData(const QModelIndex &first, const QModelIndex &last);

    QString getFilePath(int index, int parentIndex);
    int getCurrentRow(int row, int parentRow);

    int countFiles();
    void clear();

 private:
    QString getTextFromFile(int position);
    QDateTime getModificationDate(int position);
    QString getFileName(int position);

    void regenerateModel();

    int numFiles;
    QVector<Entry *> entryList;
    MLocaleBuckets buckets;
    Utils *utils;

};

#endif // FILEMODEL_H

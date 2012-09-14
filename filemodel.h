#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <MAbstractItemModel>
#include <QDateTime>

class FileModel : public MAbstractItemModel
{
    Q_OBJECT

 public:
    FileModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
   // QVariant data(const QModelIndex &index, int role) const;
    bool removeRows(int row, int count, const QModelIndex &parent);
    bool insertRows(int row, int count, const QModelIndex &parent);
    QVariant itemData(int row, int group, int role) const;
    int rowCountInGroup(int group) const;
    QString groupTitle(int group) const;
    int groupCount() const;

    QString getFilePath(int index);
    void reload();
    int countFiles();

 private:
    QString getTextFromFile(int position);
    QDateTime getModificationDate(int position);

    int numFiles;
    QList<QString> filesData;
    QList<QDateTime> lastModification;

};

#endif // FILEMODEL_H

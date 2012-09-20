#ifndef SORTDIALOGMODEL_H
#define SORTDIALOGMODEL_H

#include <QAbstractListModel>

class SortDialogModel : public QAbstractListModel
 {
    Q_OBJECT

  public:
    SortDialogModel(QObject *parent = 0)
       : QAbstractListModel(parent) {}

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;

 };

#endif // SORTDIALOGMODEL_H

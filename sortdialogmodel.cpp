#include <QStringList>

#include "sortdialogmodel.h"

int SortDialogModel::rowCount(const QModelIndex &parent) const
{
   Q_UNUSED(parent);
   return 2;
}

QVariant SortDialogModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        QStringList rowData;

        if(index.row() == 0) {
            rowData << "Modification date";
        }
        else if (index.row() == 1){
            rowData << "Name";
        }

        return QVariant(rowData);
    }

    return QVariant();
}

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

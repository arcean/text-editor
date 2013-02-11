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

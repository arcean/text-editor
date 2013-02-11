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

#ifndef SORTDIALOG_H
#define SORTDIALOG_H

#include <MDialog>
#include <MList>

class SortDialog : public MDialog
{
    Q_OBJECT
public:
    explicit SortDialog(int sortMode, QGraphicsWidget *parent = 0);

private slots:
    void itemClicked(const QModelIndex &index);

signals:
    void sortTypeChanged(int sortType);

private:
    MList *list;
};

#endif // SORTDIALOG_H

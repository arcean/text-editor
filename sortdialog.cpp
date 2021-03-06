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

#include <MLayout>
#include <MGridLayoutPolicy>
#include <MLabel>
#include <MLinearLayoutPolicy>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <MAbstractCellCreator>
#include <MBasicListItem>
#include <QStringList>
#include <MList>

#include "sortdialog.h"
#include "sortdialogmodel.h"

class MContentItemCreator : public MAbstractCellCreator<MBasicListItem>
{
public:
    void updateCell(const QModelIndex& index, MWidget *cell) const
    {
        MBasicListItem * contentItem = qobject_cast<MBasicListItem *>(cell);
        contentItem->setStyleName("CommonBasicListItemAccented");
        contentItem->setItemStyle(MBasicListItem::SingleTitle);
        QVariant data = index.data(Qt::DisplayRole);
        QStringList rowData = data.value<QStringList>();
        contentItem->setTitle(rowData[0]);
    }
};

SortDialog::SortDialog(int sortMode, QGraphicsWidget *parent)
{
    Q_UNUSED(parent);

    MWidget *centralWidget = new MWidget;
    MLayout  *layout = new MLayout();

    MLinearLayoutPolicy* landscapePolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    MLinearLayoutPolicy* portraitPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);

    layout->setLandscapePolicy(landscapePolicy);
    layout->setPortraitPolicy(portraitPolicy);

    this->setTitle("Sort by");
    centralWidget->setLayout(layout);
    this->setModal(true);

    list = new MList(this);
    list->setContentsMargins(0, 12, 0, 0);
    MContentItemCreator *cellCreator = new MContentItemCreator;
    list->setCellCreator(cellCreator);
    SortDialogModel *model = new SortDialogModel;
    list->setItemModel(model);

    // Select item
    QItemSelectionModel *selectionModel = new QItemSelectionModel(model);
    list->setSelectionModel(selectionModel);
    QModelIndex index = model->index(sortMode);
    selectionModel->select(index, QItemSelectionModel::Select);

    landscapePolicy->addItem(list);
    portraitPolicy->addItem(list);

    this->setCentralWidget(centralWidget);
selectionModel->select(index, QItemSelectionModel::Select);
    connect(list, SIGNAL(itemClicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));
}

SortDialog::~SortDialog()
{
    delete list;
}

void SortDialog::itemClicked(const QModelIndex &index)
{
    if (index.row() == 0) {
        emit this->sortTypeChanged(0);
        this->disappear();
    }
    else if (index.row() == 1) {
        emit this->sortTypeChanged(1);
        this->disappear();
    }
}

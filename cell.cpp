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
#include <MLinearLayoutPolicy>
#include "cell.h"

Cell::Cell(MWidget *parent) :
    MListItem(parent)
{
    MLayout *layout = new MLayout(this);
    MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    policy->setContentsMargins(0, 0, 0, 0);
    policy->setSpacing(0);
    policy->setNotifyWidgetsOfLayoutPositionEnabled(true);

    line1 = new MLabel();
    line2 = new MLabel();
    line3 = new MLabel();
    date = new MLabel();

    line1->setWordWrap(false);
    line2->setWordWrap(false);
    line3->setWordWrap(false);
    date->setWordWrap(false);

    line1->setTextElide(true);
    line2->setTextElide(true);
    line3->setTextElide(true);
    date->setTextElide(true);

    policy->addItem(line1);
    policy->addItem(line2);
    policy->addItem(line3);
    policy->addItem(date);

    this->setStyleName("NotesPopupListItem");
}

void Cell::setLine1(const QString& text)
{
    line1->setText(text);
}

void Cell::updateCell(const QModelIndex& index, MWidget * cell) const
{
    Cell *contentItem = qobject_cast<Cell *>(cell);
    QVariant data = index.data(Qt::DisplayRole);
    QStringList rowData = data.value<QStringList>();
    contentItem->setLine1(rowData[0]);
    //contentItem->setSubtitle("rowData[1]");
}

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

#include <QDebug>

#include "mlistitemcreator.h"

void MListItemCreator::updateCell(const QModelIndex& index, MWidget * cell) const
{
    MBasicListItem * contentItem = qobject_cast<MBasicListItem *>(cell);
    contentItem->setObjectName("CommonBasicListItem");
    contentItem->setItemStyle(MBasicListItem::TitleWithSubtitle);

    QVariant data = index.data(Qt::DisplayRole);
    QStringList rowData = data.value<QStringList>();
    QString title = rowData[0];


    if (!highlightText.isEmpty()) {
        int matchingIndex = 0;
        while (matchingIndex > -1) {
            matchingIndex = title.indexOf(highlightText, matchingIndex, Qt::CaseInsensitive);
            if (matchingIndex != -1) {
                title.insert(matchingIndex + highlightText.length(), "</font></u>");
                title.insert(matchingIndex, "<u><font color=#EFAF11>");
                matchingIndex += highlightText.length() + 11 + 23;
            }
        }
    }

    contentItem->setTitle(title);
    contentItem->setSubtitle(rowData[1]);
}

void MListItemCreator::highlightByText(const QString &text)
{
    highlightText = text;
}

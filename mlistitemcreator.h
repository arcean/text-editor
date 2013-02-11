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

#ifndef MLISTITEMCREATOR_H
#define MLISTITEMCREATOR_H

#include <MAbstractCellCreator>
#include <MBasicListItem>

class MListItemCreator : public MAbstractCellCreator<MBasicListItem>
{
public:
    void updateCell(const QModelIndex& index, MWidget * cell) const;
    void highlightByText(const QString &text);

private:
    QString highlightText;

};

#endif // MLISTITEMCREATOR_H

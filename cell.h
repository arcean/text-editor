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

#ifndef CELL_H
#define CELL_H

#include <MWidget>
#include <QModelIndex>
#include <MLabel>
#include <MListItem>

class Cell : public MListItem
{
    Q_OBJECT
public:
    explicit Cell(MWidget *parent = 0);
    ~Cell();

    void updateCell(const QModelIndex& index, MWidget * cell) const;

    void setLine1(const QString& text);
    
signals:
    
public slots:

private:
    MLabel *line1;
    MLabel *line2;
    MLabel *line3;
    MLabel *date;
    
};

#endif // CELL_H

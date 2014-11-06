/***************************************************************************
**
** Copyright (C) 2012 - 2014 Tomasz Pieniążek
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

#include <MButton>

#include "confirmdeletedialog.h"

ConfirmDeleteDialog::ConfirmDeleteDialog(const QString &fileName):
    m_fileName(fileName)
{
    // Little hack to get rid of OK button
    MButtonModel *buttonModel = button(M::OkButton);

    if (buttonModel)
        removeButton(buttonModel);

    // As we want to have only Yes/No buttons.
    this->addButton(M::YesButton);
    this->addButton(M::NoButton);


    setTitle(qtTrId("Are you sure?"));
    if (fileName.size() > 50)
        setText(qtTrId("Are you sure that you want to delete this note ?"));
    else
        setText(qtTrId("Are you sure that you want to delete \"" + m_fileName.toAscii() + "\" ?"));

    QPixmap icon("/opt/exnote/data/dialog-question.png");
    setIconPixmap(icon);
}

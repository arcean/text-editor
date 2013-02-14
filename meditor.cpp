/***************************************************************************
**
** Copyright (C) 2013 Tomasz Pieniążek
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

#include "meditor.h"

MEditor::MEditor(QGraphicsItem *parent)
    : MWidgetController(parent)
{
    this->setObjectName("NoteWidgetStyle");
    mainLayout = new QGraphicsLinearLayout(Qt::Vertical, this);
    mainLayout->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    timeStamp = new MTimeStampWidget(this);
    mainLayout->addItem(timeStamp);

    textEditor = new MRichTextEdit(MTextEditModel::MultiLine);
    textEditor->setStyleName("CommonEditorNoteArea");
    textEditor->setPrompt("Add a note");
    mainLayout->addItem(textEditor);
}

MEditor::~MEditor()
{

}

MRichTextEdit* MEditor::getTextEditor()
{
    return this->textEditor;
}

void MEditor::setFocus(Qt::FocusReason focusReason)
{
    this->textEditor->setFocus(focusReason);
}

void MEditor::setDateTime(const QString &text)
{
    this->timeStamp->setDateTime(text);
}

void MEditor::setAutoDateTime()
{
    this->timeStamp->setAutoDateTime();
}

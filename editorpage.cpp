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
#include <MPannableViewport>
#include <QGraphicsLinearLayout>
#include <MLinearLayoutPolicy>
#include <MLabel>
#include <MRichTextEdit>
#include <MAction>
#include <QDateTime>
#include <QDir>
#include <QDebug>

#include "editorpage.h"
#include "viewHeader.h"

EditorPage::EditorPage(bool focusOnEditor, QGraphicsItem *parent)
{
    Q_UNUSED(parent);

    /* Sheet header */
    MBasicSheetHeader *sheetHeader = new MBasicSheetHeader;

    saveAction = new QAction("Save", sheetHeader);
    sheetHeader->setPositiveAction(saveAction);
    sheetHeader->setNegativeAction(new QAction("Cancel", sheetHeader));

    setHeaderWidget(sheetHeader);

    // Create main app viewport
    MPannableViewport *viewportWidget = new MPannableViewport;
    this->setCentralWidget(viewportWidget);

    /////////////////////////////////////////////////// ACTIONS

    /////////////////////////////////////////////////// CONTENT
    editor = new MRichTextEdit(MTextEditModel::MultiLine);
    viewportWidget->setWidget(editor);

    /////////////////////////////////////////////////// SIGNALS
    connect(sheetHeader->negativeAction(), SIGNAL(triggered()), SLOT(processDialogRejected()));
    connect(sheetHeader->positiveAction(), SIGNAL(triggered()), SLOT(processDialogAccepted()));
    connect(editor, SIGNAL(textChanged()), this, SLOT(enableSaveButton()));

    if (focusOnEditor)
        connect(this, SIGNAL(appeared()), this, SLOT(setFocusOnEditor()));

    /////////////////////////////////////////////////// OTHER
    // Disable "Save" button
    saveAction->setDisabled(true);
}

void EditorPage::setFocusOnEditor()
{
    // Focus on the editor
    editor->setFocus(Qt::OtherFocusReason);
}

void EditorPage::enableSaveButton()
{
    bool disable = editor->text().length() > 0 ? false : true;

    saveAction->setDisabled(disable);
}

void EditorPage::processDialogRejected()
{
    this->close();
}

void EditorPage::processDialogAccepted()
{
    if (editor->text().length() > 0)
        writeToFile();

    this->close();
}

void EditorPage::loadFile(const QString& filePath, int filePosition)
{
    QFile file (filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "[E]: could not open the file" << filePath;
        return;
    }

    QTextStream in(&file);
    QString line = in.readLine();

    while (!in.atEnd()) {
        line += in.readLine();
    }

    editor->setHtml(line);
    this->fileName = filePath;
    this->filePosition = filePosition;
}

void EditorPage::writeToFile()
{
    QString text = editor->text();
    QString oldFileName = fileName;

    /* Create a new filename based on the note's text. */
    Utils::getNewFilename(text);
    text = "/home/user/MyDocs/exnote/" + text;
    fileName = text;

    QFile file(fileName);

    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << editor->toHtml();
    file.close();

    QFile oldFile(oldFileName);

    if (oldFile.exists())
        oldFile.remove();
    oldFile.close();

    if (oldFileName.length() > 0)
        emit reloadModel(filePosition);
    else
        emit reloadModel(-1);
}

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

#ifndef EDITORPAGE_H
#define EDITORPAGE_H

#include <QObject>
#include <MSheet>
#include <MRichTextEdit>
#include <MBasicSheetHeader>

#include "utils.h"
#include "singleton.h"
#include "meditor.h"

class EditorPage : public MSheet
{
    Q_OBJECT
public:
    explicit EditorPage(bool focusOnEditor = false, QGraphicsItem *parent = 0);

    void loadFile(const QString& filePath, int filePosition);

signals:
    void reloadModel(int filePosition);
    
public slots:

private slots:
    void processDialogRejected();
    void processDialogAccepted();
    void enableSaveButton();
    void setFocusOnEditor();

private:
    void writeToFile();

    QAction *saveAction;
    //MRichTextEdit *editor;
    MEditor *editor;
    // Current filename
    QString fileName;
    int filePosition;
    
};

#endif // EDITORPAGE_H

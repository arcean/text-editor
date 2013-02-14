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

#ifndef MEDITOR_H
#define MEDITOR_H

#include <MWidgetController>
#include <MRichTextEdit>
#include <QObject>
#include <QGraphicsLinearLayout>

#include "mtimestampwidget.h"

class MEditor : public MWidgetController
{
    Q_OBJECT

public:
    MEditor(QGraphicsItem *parent = 0);
    ~MEditor();

    MRichTextEdit* getTextEditor();

    void setFocus(Qt::FocusReason focusReason);

    void setDateTime(const QString &text);
    void setAutoDateTime();

public slots:

private slots:

signals:

private:
    QGraphicsLinearLayout *mainLayout;

    MRichTextEdit *textEditor;
    MTimeStampWidget *timeStamp;

};

#endif // MEDITOR_H

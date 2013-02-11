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

#ifndef MTEXTEDITFILTER_H
#define MTEXTEDITFILTER_H

#include <MWidgetController>
#include <MTextEdit>
#include <MButton>
#include <QGraphicsLinearLayout>
#include <QTimer>

#include "WidgetAnimationIn.h"
#include "WidgetAnimationOut.h"

class MTextEditFilter : public MWidgetController
{
    Q_OBJECT

public:
    MTextEditFilter();

    void connectAll();
    void resetTimer();

    MTextEdit *editor;

public slots:
    void showWidget();
    void hideWidget();

private slots:
    void select(Qt::FocusReason);
    void deselect(Qt::FocusReason);

    void searchButtonClicked();
    void textChanged();

    void searchButtonAnimationFinished();
    void searchButtonAnimationStart();

    void widgetIsVisible();
    void widgetIsInvisible();

signals:
    void widgetShown();
    void widgetHidden();

private:
    MButton *searchButton;
    QGraphicsLinearLayout *mainLayout;

    // Animations
    WidgetAnimationIn *searchButtonAnimationIn;
    WidgetAnimationOut *searchButtonAnimationOut;
    WidgetAnimationIn *widgetAnimationIn;
    WidgetAnimationOut *widgetAnimationOut;
    bool nextClearIcon;
    bool isClearIcon;

    QTimer *widgetTimer;
};

#endif // MTEXTEDITFILTER_H

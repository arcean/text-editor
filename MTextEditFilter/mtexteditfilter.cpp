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

#include "mtexteditfilter.h"

MTextEditFilter::MTextEditFilter()
{
    this->setObjectName("FilteringTextEditContainer");
    mainLayout = new QGraphicsLinearLayout(Qt::Horizontal, this);
    mainLayout->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    searchButton = new MButton("icon-m-common-search", "");
    searchButton->setViewType(MButton::iconType);
    searchButton->setObjectName("FilteringButton");

    searchButtonAnimationIn = new WidgetAnimationIn(searchButton);
    searchButtonAnimationOut = new WidgetAnimationOut(searchButton);
    widgetAnimationIn = new WidgetAnimationIn(this);
    widgetAnimationOut = new WidgetAnimationOut(this);
    nextClearIcon = true;
    isClearIcon = false;

    widgetTimer = new QTimer(this);
    widgetTimer->setInterval(5000);

    connect (searchButton, SIGNAL(clicked()), this, SLOT(searchButtonClicked()));
    connect (searchButtonAnimationOut, SIGNAL(finished()), this, SLOT(searchButtonAnimationFinished()));
    connect(widgetTimer, SIGNAL(timeout()), this, SLOT(hideWidget()));
}

void MTextEditFilter::resetTimer()
{
    if (widgetTimer->isActive())
        widgetTimer->stop();
    widgetTimer->start();
}

// The widget is visible, start counting time to hide
void MTextEditFilter::widgetIsVisible()
{
    emit widgetShown();
    widgetTimer->start();
}

// The widget is invisible
void MTextEditFilter::widgetIsInvisible()
{
    emit widgetHidden();
}

// Starts hide animation
void MTextEditFilter::hideWidget()
{
    widgetTimer->stop();
    widgetIsInvisible();
}

// Starts show animation
void MTextEditFilter::showWidget()
{
    emit widgetShown();
    widgetTimer->start();
}

void MTextEditFilter::connectAll()
{
    editor->setStyleName("");
    editor->setObjectName("CommonSingleInputField");
    editor->setPrompt("Search");
    editor->setAutoSelectionEnabled(false);
    editor->setInputMethodPredictionEnabled(false);
    editor->setErrorHighlight(false);

    mainLayout->addItem(editor);
    mainLayout->addItem(searchButton);

    connect (editor, SIGNAL(gainedFocus(Qt::FocusReason)), this, SLOT(select(Qt::FocusReason)));
    connect (editor, SIGNAL(lostFocus(Qt::FocusReason)), this, SLOT(deselect(Qt::FocusReason)));
    connect (editor, SIGNAL(textChanged()), this, SLOT(textChanged()));
}

void MTextEditFilter::searchButtonAnimationStart()
{
    searchButtonAnimationOut->start();
}

void MTextEditFilter::searchButtonAnimationFinished()
{
    if (nextClearIcon) {
        searchButton->setIconID("icon-m-input-clear");
        isClearIcon = true;
    }
    else {
        searchButton->setIconID("icon-m-common-search");
        isClearIcon = false;
    }

    searchButtonAnimationIn->start();
}

void MTextEditFilter::textChanged()
{
    if (editor->text().length() > 0) {
        nextClearIcon = true;
        widgetTimer->stop();
    }
    else {
        nextClearIcon = false;
        widgetTimer->start();
    }

    if (nextClearIcon && isClearIcon)
        return;

    searchButtonAnimationStart();
}

void MTextEditFilter::searchButtonClicked()
{
    editor->setFocus();

    if (editor->text().length() > 0)
        editor->setText("");
}

void MTextEditFilter::select(Qt::FocusReason)
{
    this->setSelected(true);
}

void MTextEditFilter::deselect(Qt::FocusReason)
{
    this->setSelected(false);
}

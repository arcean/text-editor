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

#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <MApplicationPage>
#include <MDialog>
#include <QColor>
#include <MAction>
#include <MWidgetAction>
#include <MLabel>
#include <MBanner>
#include <MList>
#include <MObjectMenu>
#include <MLinearLayoutPolicy>
#include <MSortFilterProxyModel>
#include <MBasicLayoutAnimation>

#include "filemodel.h"
#include "mlistitemcreator.h"
#include "ClickableViewHeader.h"

#include "singleton.h"
#include "settings.h"
#include "MTextEditFilter/mtexteditfilter.h"

/* To enable debug msgs: */
/* -output-level debug */

class MainPage : public MApplicationPage {
    Q_OBJECT
public:
    MainPage(QGraphicsItem *parent = 0);
    ~MainPage();

protected:
    virtual void createContent();

public slots:

private slots:
    void showAboutDialog();
    void showSortDialog();

    void parseSortDialogOutput(int sortType);

    void showNewEditor();
    void showEditor(const QModelIndex& index);

    void reloadModel(int filePosition);

    void showObjectMenu(const QModelIndex &index);

    void deleteAccepted();
    void showConfirmDeleteDialog();
    void showShareDialog();

    // MTextEditFilter and MPannableWidget
    void listPannedUpFromTop();
    // Removes widget from the window after the animation is finished
    void removeEditFilterWidget();

    void decideNoNotesLabel();
    void listPanningStarted();

    void highlightFilteredText();

private:
    void throwMessage(const QString &text);
    void removeNoteSlot();

    void insertNoNotesLabel();
    void insertNothingFoundLabel();

    MBanner *infoBanner;
    MButton *installButton;
    MButton *aboutButton;

    MLinearLayoutPolicy *viewportLayoutPolicy;
    QGraphicsLinearLayout *mainLayout;
    MPannableViewport *viewportWidget;

    MLayout *viewportLayout;
    QGraphicsWidget *form;

    MLabel *noNotesLabel;
    bool isListVisible;

    FileModel *model;
    MListItemCreator *cellCreator;

    MList *list;
    MObjectMenu *objectMenu;
    QModelIndex longTappedIndex;
    MSortFilterProxyModel *proxyModel;

    MTextEditFilter *editFilter;
    bool isEditFilterInserted;

    // Animations
    MBasicLayoutAnimation *layoutAnimation;
    WidgetAnimationIn *listAnimationIn;
    WidgetAnimationOut *listAnimationOut;

};

#endif // MAINPAGE_H

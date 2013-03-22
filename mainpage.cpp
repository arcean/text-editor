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

#include <QGraphicsLinearLayout>
#include <MButton>
#include <MLayout>
#include <MToolBar>
#include <MAction>
#include <MTextEdit>
#include <MSceneWindow>
#include <QDebug>
#include <MApplication>
#include <MOnDisplayChangeEvent>
#include <MApplicationWindow>
#include <MTheme>
#include <QGraphicsGridLayout>
#include <MGridLayoutPolicy>
#include <MImageWidget>
#include <MPannableViewport>
#include <MSeparator>
#include <QGraphicsItem>
#include <MMessageBox>
#include <math.h>
#include <MAbstractCellCreator>
#include <MList>
#include <MBasicListItem>
#include <QFile>
#include <MSceneManager>
#include <MListFilter>
#include <MPositionIndicator>

#include "mainpage.h"
#include "editorpage.h"
#include "mlistitemcreator.h"
#include "aboutdialog.h"
#include "confirmdeletedialog.h"
#include "sortdialog.h"
#include "sharecommand.h"

MainPage::MainPage(QGraphicsItem *parent)
    : MApplicationPage(parent)
{

}

MainPage::~MainPage()

{
    delete model;
    delete objectMenu;
}

void MainPage::createContent()
{
    MTheme *theme = MTheme::instance();
    theme->addPixmapDirectory("/opt/exnote/data/");
    theme->loadCSS("/opt/exnote/style/exnote.css");
    applicationWindow()->setStyleName("CommonApplicationWindow");
    setStyleName("CommonApplicationPage");
    // "this->centralWidget()" instead of "this" fixes overlaping list by toolbar:
    QGraphicsLinearLayout *mainLayout = new QGraphicsLinearLayout(Qt::Vertical, this->centralWidget());
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    /////////////////////////////////////////////////// HEADER VIEW

    /* Fix: lock page content */
    setPannable(false);
    MPannableViewport *viewport = this->pannableViewport();
    viewport->setAutoRange(false);
    viewport->setRange(QRectF(0,0,0,0));

    ClickableViewHeader *header = new ClickableViewHeader;
    header->setTitle("exNote");

    // Create main app viewport
    viewportWidget = new MPannableViewport;
    // Create layout for the main app viewport
    MLayout *viewportLayout = new MLayout();
    layoutAnimation = new MBasicLayoutAnimation(viewportLayout);

    // Create widget for integrating the layout with the viewport
    QGraphicsWidget *form = new QGraphicsWidget();
    form->setLayout(viewportLayout);
    viewportWidget->setWidget(form);
    // Create layout policy for the main app viewport
    viewportLayoutPolicy = new MLinearLayoutPolicy(viewportLayout, Qt::Vertical);
    viewportLayoutPolicy->setObjectName("ListViewport");
    viewportLayoutPolicy->setContentsMargins(0, 0, 0, 0);
    viewportLayoutPolicy->setSpacing(0);
    viewportLayoutPolicy->setNotifyWidgetsOfLayoutPositionEnabled(true);
    // Add header to the layout
    mainLayout->addItem(header);
    // Add viewportWidget to layout (main layout)
    mainLayout->addItem(viewportWidget);

    /////////////////////////////////////////////////// SETTINGS SINGLETON

    /////////////////////////////////////////////////// CONTENT

    list = new MList();
    list->setObjectName("ListStyle");
    cellCreator = new MListItemCreator;
    list->setCellCreator(cellCreator);
    model = new FileModel();
    // PROXY MODEL
    proxyModel = new MSortFilterProxyModel();
    proxyModel->setSortRole(FileModel::EntrySortRole);
    proxyModel->setFilterRole(FileModel::EntryFilterRole);
    proxyModel->setDynamicSortFilter(true);
    proxyModel->setSourceModel(model);

    list->setItemModel(proxyModel);

    // Load sorting settings
    model->setGrouped(true);
    list->setShowGroups(true);
    list->setIndexDisplayMode(MList::Auto);
    list->setAutoFillBackground(false);

    // MTextEditFilter
    editFilter = new MTextEditFilter();
    editFilter->editor = list->filtering()->editor();
    editFilter->connectAll();
    list->filtering()->setEnabled(true);
    list->filtering()->setFilterRole(FileModel::EntryFilterRole);
    list->filtering()->editor()->setVisible(false);
    isEditFilterInserted = false;

    noNotesLabel = new MLabel("Add your first note");
    noNotesLabel->setStyleName("CommonEmptyStateTitle");
    noNotesLabel->setAlignment(Qt::AlignCenter);
    noNotesLabel->setWordWrap(true);
    noNotesLabel->setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);

    if (model->countFiles() > 0) {
        viewportLayoutPolicy->insertItem(2, list);
        isListVisible = true;
    }
    else {
        viewportLayoutPolicy->insertItem(1, noNotesLabel);
        isListVisible = false;
    }

    /////////////////////////////////////////////////// ACTIONS
    MAction *dummyAction = new MAction("", "", this);
    dummyAction->setLocation(MAction::ToolBarLocation);
    this->addAction(dummyAction);

    MAction *addNewNote = new MAction("icon-m-toolbar-add", "Add", this);
    addNewNote->setLocation(MAction::ToolBarLocation);
    this->addAction(addNewNote);

    MAction *aboutDialog = new MAction("icon-m-toolbar-tag", "Add", this);
    aboutDialog->setLocation(MAction::ToolBarLocation);
    this->addAction(aboutDialog);

    /////////////////////////////////////////////////// OBJECT MENU
    objectMenu = new MObjectMenu(0);

    MAction *shareNoteFile = new MAction("Share as a file", this);
    shareNoteFile->setLocation(MAction::ObjectMenuLocation);
    objectMenu->addAction(shareNoteFile);
    MAction *shareNoteText = new MAction("Share as a text", this);
    shareNoteText->setLocation(MAction::ObjectMenuLocation);
    objectMenu->addAction(shareNoteText);
    MAction *removeNote = new MAction("Remove", this);
    removeNote->setLocation(MAction::ObjectMenuLocation);
    objectMenu->addAction(removeNote);

    /////////////////////////////////////////////////// SIGNALS
    connect(addNewNote, SIGNAL(triggered()), this, SLOT(showNewEditor()));
    connect(list, SIGNAL(itemLongTapped(QModelIndex)), this, SLOT(showObjectMenu(QModelIndex)));
    connect(list, SIGNAL(itemClicked(QModelIndex)), this, SLOT(showEditor(QModelIndex)));
    connect(removeNote, SIGNAL(triggered()), this, SLOT(showConfirmDeleteDialog()));
    connect(shareNoteFile, SIGNAL(triggered()), this, SLOT(showShareDialog()));
    connect(shareNoteText, SIGNAL(triggered()), this, SLOT(showShareTextDialog()));
    connect(aboutDialog, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
    connect(header, SIGNAL(clicked()), this, SLOT(showSortDialog()));
    connect(list->filtering(), SIGNAL(listPannedUpFromTop()), this, SLOT(listPannedUpFromTop()));
    connect(editFilter, SIGNAL(widgetHidden()), this, SLOT(removeEditFilterWidget()));
    connect(list, SIGNAL(geometryChanged()), this, SLOT(decideNoNotesLabel()));
    connect(list, SIGNAL(panningStarted()), this, SLOT(listPanningStarted()));
   // connect(list->filtering()->editor(), SIGNAL(textChanged()), this, SLOT(highlightFilteredText()));

    /////////////////////////////////////////////////// OTHER
    // FIXME: implement banners.
    // Create info banner.
    //infoBanner = new MBanner();
}

void MainPage::highlightFilteredText()
{
    // FIXME:
    //cellCreator->highlightByText(list->filtering()->editor()->text());
}

void MainPage::listPanningStarted()
{
    list->setFocus();
}

void MainPage::insertNoNotesLabel()
{
    viewportLayoutPolicy->removeItem(list);
    viewportLayoutPolicy->removeItem(noNotesLabel);
    noNotesLabel->setText("Add your first note");
    viewportLayoutPolicy->insertItem(1, noNotesLabel);
    viewportLayoutPolicy->setStretchFactor(noNotesLabel, 5);
}

void MainPage::insertNothingFoundLabel()
{
    viewportLayoutPolicy->removeItem(noNotesLabel);
    noNotesLabel->setText("No notes found");
    viewportLayoutPolicy->removeItem(list);
    viewportLayoutPolicy->insertItem(1, noNotesLabel);
    viewportLayoutPolicy->setStretchFactor(noNotesLabel, 0);
    viewportLayoutPolicy->insertItem(2, list);
}

// Removes the widget from the window after the animation is finished.
void MainPage::removeEditFilterWidget()
{
    list->filtering()->editor()->setText("");
    list->filtering()->editor()->setVisible(false);
    viewportLayoutPolicy->removeItem(editFilter);
    isEditFilterInserted = false;
}

//  Decides if we should show editFilter widget
void MainPage::listPannedUpFromTop()
{
    // Do not show the editFilter widget the list is empty.
    bool itemsAvailable = model->countFiles() > 0 ? true : false;

    if (!isEditFilterInserted && itemsAvailable) {
        isEditFilterInserted = true;
        list->filtering()->editor()->setVisible(true);
        viewportLayoutPolicy->insertItem(0, editFilter);
        editFilter->showWidget();
    }
}

void MainPage::showSortDialog()
{
    SortDialog *sortDialog = new SortDialog(Settings::getSortMode());

    connect(sortDialog, SIGNAL(sortTypeChanged(int)), this, SLOT(parseSortDialogOutput(int)));
    sortDialog->appear(MSceneWindow::DestroyWhenDone);
}

void MainPage::parseSortDialogOutput(int sortType)
{
    Settings::setSortMode(sortType);
    model->changeModel();
}

void MainPage::showAboutDialog()
{
    AboutDialog *aboutDialog = new AboutDialog();
    aboutDialog->appear(MSceneWindow::DestroyWhenDone);
}

void MainPage::decideNoNotesLabel()
{
    bool itemsAvailable = model->countFiles() > 0 ? true : false;
    bool isHeightOk = list->geometry().height() > 0;

    if (!itemsAvailable && isListVisible) {
        insertNoNotesLabel();
        isListVisible = false;
    }
    else if (itemsAvailable && isListVisible && !isHeightOk) {
        insertNothingFoundLabel();
        isListVisible = false;
    }
    else if (itemsAvailable && !isListVisible && isHeightOk) {
        viewportLayoutPolicy->removeItem(noNotesLabel);
        viewportLayoutPolicy->removeItem(list);
        viewportLayoutPolicy->insertItem(2, list);
        isListVisible = true;
    }
}

void MainPage::reloadModel(int filePosition)
{
    removeEditFilterWidget();

    if (!isListVisible)
        viewportLayoutPolicy->insertItem(2, list);

    if (filePosition == -1)
        list->itemModel()->insertRows(0, 1, QModelIndex());
    else {
        /* Firstly, remove current row.
         * Strangely the following line sometimes doesn't work.
         */
        //list->itemModel()->removeRows(filePosition, 1, QModelIndex());
        model->removeRows(filePosition, 1, QModelIndex());

        // Then insert a new one at 0.
        list->itemModel()->insertRows(0, 1, QModelIndex());
    }

    if (!isListVisible)
        viewportLayoutPolicy->removeAt(2);

    decideNoNotesLabel();
}

void MainPage::showObjectMenu(const QModelIndex &index)
{
    // Show objectMenu.
    sceneManager()->appearSceneWindow(objectMenu);
    longTappedIndex = index;
}

void MainPage::removeNoteSlot()
{
    if(longTappedIndex.isValid()) {
        QVariant data = longTappedIndex.data(Qt::DisplayRole);
        QStringList rowData = data.value<QStringList>();
        QString filePath = rowData[2];
        QFile file(filePath);

        file.remove();
        list->itemModel()->removeRows(longTappedIndex.row(), 1, longTappedIndex.parent());

        //FIXME: workaround for a bug with disappearing items in the list (groups thing):
        proxyModel->setSourceModel(model);

        longTappedIndex = QModelIndex();

        if (model->countFiles() == 0)
            removeEditFilterWidget();
        decideNoNotesLabel();
    }
}

void MainPage::deleteAccepted()
{
    removeNoteSlot();
}

void MainPage::showConfirmDeleteDialog()
{
    ConfirmDeleteDialog *dialog = new ConfirmDeleteDialog();

    connect(dialog, SIGNAL(accepted()), this, SLOT(deleteAccepted()));

    dialog->appear(MSceneWindow::DestroyWhenDone);
}

void MainPage::showShareDialog()
{
    if(longTappedIndex.isValid()) {
        QVariant data = longTappedIndex.data(Qt::DisplayRole);
        QStringList rowData = data.value<QStringList>();
        QString filePath = rowData[2];

        ShareCommand shareCommand;
        shareCommand.share(filePath);

        longTappedIndex = QModelIndex();
    }
}

void MainPage::showShareTextDialog()
{
    if(longTappedIndex.isValid()) {
        QVariant data = longTappedIndex.data(Qt::DisplayRole);
        QStringList rowData = data.value<QStringList>();
        QString title = rowData[0];
        // rowData[2] == filePath
        QString text = Utils::readDataToTxt(rowData[2]);
        qDebug() << "ret2" << text;
        qDebug() << "title" << title;

        ShareCommand shareCommand;
        shareCommand.shareAsText(text, title);

        longTappedIndex = QModelIndex();
    }
}

void MainPage::showEditor(const QModelIndex& index)
{
    QVariant data = index.data(Qt::DisplayRole);
    QStringList rowData = data.value<QStringList>();
    QString filePath = rowData[2];
    int filePosition = rowData[3].toInt();
    EditorPage *editor = new EditorPage();

    editor->loadFile(filePath, filePosition);

    connect(editor, SIGNAL(reloadModel(int)), this, SLOT(reloadModel(int)));

    editor->appear(MSceneWindow::DestroyWhenDismissed);
}

void MainPage::showNewEditor()
{
    // Focus on the text entry
    EditorPage *editor = new EditorPage(true);

    connect(editor, SIGNAL(reloadModel(int)), this, SLOT(reloadModel(int)));

    editor->appear(MSceneWindow::DestroyWhenDismissed);
}

void MainPage::throwMessage(const QString &text)
{
    infoBanner->setStyleName(MBannerType::InformationBanner);
    infoBanner->setTitle(text);
    infoBanner->appear(scene(), MSceneWindow::KeepWhenDone);
}

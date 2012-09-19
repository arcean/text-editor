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

#include "mainpage.h"
#include "viewHeader.h"
#include "editorpage.h"
#include "mlistitemcreator.h"
#include "aboutdialog.h"
#include "confirmdeletedialog.h"

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
    theme->loadCSS("/opt/exnote/style/exnote.css");
    applicationWindow()->setStyleName("CommonApplicationWindow");
    setStyleName("CommonApplicationPage");
    MLayout *layout = new MLayout(this);

    /////////////////////////////////////////////////// HEADER VIEW

    /* Fix: lock page content */
    setPannable(false);
    MPannableViewport *viewport = this->pannableViewport();
    viewport->setAutoRange(false);
    viewport->setRange(QRectF(0,0,0,0));

    ViewHeader *header = new ViewHeader;
    header->setTitle("exNote");

    MLinearLayoutPolicy *portraitPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    layout->setPortraitPolicy(portraitPolicy);
    portraitPolicy->setContentsMargins(0, 0, 0, 0);
    portraitPolicy->setSpacing(0);
    portraitPolicy->setNotifyWidgetsOfLayoutPositionEnabled(true);
    // Create main app viewport
    MPannableViewport *viewportWidget = new MPannableViewport;
    // Create layout for the main app viewport
    MLayout *viewportLayout = new MLayout();
    // Create widget for integrating the layout with the viewport
    QGraphicsWidget *form = new QGraphicsWidget();
    form->setLayout(viewportLayout);
    viewportWidget->setWidget(form);
    // Create layout policy for the main app viewport
    viewportLayoutPolicy = new MLinearLayoutPolicy(viewportLayout, Qt::Vertical);
    viewportLayoutPolicy->setObjectName("ListViewport");
    // Add header to the layout
    portraitPolicy->addItem(header);
    // Add viewportWidget to layout (main layout)
    portraitPolicy->addItem(viewportWidget);

    /////////////////////////////////////////////////// CONTENT

    list = new MList(this);
    list->setObjectName("ListStyle");
    MListItemCreator *cellCreator = new MListItemCreator;
    list->setCellCreator(cellCreator);
    model = new FileModel();
    // PROXY MODEL
    proxyModel = new MSortFilterProxyModel();
    proxyModel->setSortRole(FileModel::EntrySortRole);
    proxyModel->setFilterRole(FileModel::EntryFilterRole);
    proxyModel->setDynamicSortFilter(true);
    proxyModel->setSourceModel(model);

    list->setItemModel(proxyModel);
    //model->setGrouped(true);
    list->setShowGroups(true);
    list->setIndexDisplayMode(MList::Auto);

    noNotesLabel = new MLabel("Add your first note");
    noNotesLabel->setStyleName("CommonEmptyStateTitle");
    noNotesLabel->setAlignment(Qt::AlignCenter);
    noNotesLabel->setWordWrap(true);
    noNotesLabel->setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);

    if (model->countFiles() > 0) {
        viewportLayoutPolicy->addItem(list);
        isListVisible = true;
    }
    else {
        viewportLayoutPolicy->addItem(noNotesLabel);
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

    MAction *removeNote = new MAction("Remove", this);
    removeNote->setLocation(MAction::ObjectMenuLocation);
    objectMenu->addAction(removeNote);

    /////////////////////////////////////////////////// SIGNALS
    connect(addNewNote, SIGNAL(triggered()), this, SLOT(showNewEditor()));
    connect(list, SIGNAL(itemLongTapped(QModelIndex)), this, SLOT(showObjectMenu(QModelIndex)));
    connect(list, SIGNAL(itemClicked(QModelIndex)), this, SLOT(showEditor(QModelIndex)));
    connect(removeNote, SIGNAL(triggered()), this, SLOT(showConfirmDeleteDialog()));
    connect(aboutDialog, SIGNAL(triggered()), this, SLOT(showAboutDialog()));

    /////////////////////////////////////////////////// OTHER
    // Create info banner.
    infoBanner = new MBanner();
}

void MainPage::showAboutDialog()
{
    AboutDialog *aboutDialog = new AboutDialog();
    aboutDialog->appear(MSceneWindow::DestroyWhenDone);
}

void MainPage::decideNoNotesLabel()
{
    bool show = model->countFiles() > 0 ? true : false;

    if (!show && isListVisible) {
        viewportLayoutPolicy->removeItem(list);
        viewportLayoutPolicy->addItem(noNotesLabel);
        isListVisible = false;
    }
    else if (show && !isListVisible) {
        viewportLayoutPolicy->removeItem(noNotesLabel);
        viewportLayoutPolicy->addItem(list);
        isListVisible = true;
    }
}

void MainPage::reloadModel(int oldRow)
{
    if (oldRow == -1)
        list->itemModel()->insertRows(0, 1, QModelIndex());
    else {
        // Firstly, remove current row
        list->itemModel()->removeRows(oldRow, 1, QModelIndex());
        // Then insert a new one at 0
        list->itemModel()->insertRows(0, 1, QModelIndex());
    }

    decideNoNotesLabel();
}

void MainPage::showObjectMenu(const QModelIndex &index)
{
    // Show objectMenu
    sceneManager()->appearSceneWindow(objectMenu);
    longTappedIndex = index;
}

void MainPage::removeNoteSlot()
{
    if(longTappedIndex.isValid()) {
        QString filePath = model->getFilePath(longTappedIndex.row(), longTappedIndex.parent().row());
        QFile file(filePath);
        file.remove();

        //list->itemModel()->removeRow(longTappedIndex.row(), longTappedIndex.parent());
        list->itemModel()->removeRows(longTappedIndex.row(), 1, longTappedIndex.parent());
        longTappedIndex = QModelIndex();

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

void MainPage::showEditor(const QModelIndex& index)
{
    QString filePath = model->getFilePath(index.row(), index.parent().row());

    EditorPage *editor = new EditorPage();
    editor->loadFile(filePath, index.row());

    connect(editor, SIGNAL(reloadModel(int)), this, SLOT(reloadModel(int)));
    editor->appear(MSceneWindow::DestroyWhenDismissed);
}

void MainPage::showNewEditor()
{
    EditorPage *editor = new EditorPage();
    connect(editor, SIGNAL(reloadModel(int)), this, SLOT(reloadModel(int)));
    editor->appear(MSceneWindow::DestroyWhenDismissed);
    editor->setFocusOnEditor();
}

void MainPage::throwMessage(const QString &text)
{
    infoBanner->setStyleName(MBannerType::InformationBanner);
    infoBanner->setTitle(text);
    infoBanner->appear(scene(), MSceneWindow::KeepWhenDone);
}

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

#include "filemodel.h"
#include "ClickableViewHeader.h"

#include "singleton.h"
#include "settings.h"

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

    void reloadModel(int oldRow, int oldParentRow);

    void showObjectMenu(const QModelIndex &index);

    void deleteAccepted();
    void showConfirmDeleteDialog();

private:
    void throwMessage(const QString &text);
    void removeNoteSlot();
    void decideNoNotesLabel();

    Settings *settings;

    MBanner *infoBanner;
    MButton *installButton;
    MButton *aboutButton;

    MLinearLayoutPolicy *viewportLayoutPolicy;

    MLabel *noNotesLabel;
    bool isListVisible;

    FileModel *model;

    MList *list;
    MObjectMenu *objectMenu;
    QModelIndex longTappedIndex;
    MSortFilterProxyModel *proxyModel;

};

#endif // MAINPAGE_H

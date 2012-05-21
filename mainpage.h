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

#include "filemodel.h"

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

    void showNewEditor();
    void showEditor(const QModelIndex& index);

    void reloadModel(int oldRow);

    void showObjectMenu(const QModelIndex &index);

    void deleteAccepted();
    void showConfirmDeleteDialog();

private:
    void throwMessage(const QString &text);

    void removeNoteSlot();

    MBanner *infoBanner;
    MButton *installButton;
    MButton *aboutButton;

    FileModel *model;

    MList *list;
    MObjectMenu *objectMenu;
    QModelIndex longTappedIndex;

};

#endif // MAINPAGE_H

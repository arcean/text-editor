#ifndef SORTDIALOG_H
#define SORTDIALOG_H

#include <MDialog>
#include <MList>

class SortDialog : public MDialog
{
    Q_OBJECT
public:
    explicit SortDialog(int sortMode, QGraphicsWidget *parent = 0);

private slots:
    void itemClicked(const QModelIndex &index);

signals:
    void sortTypeChanged(int sortType);

private:
    MList *list;
};

#endif // SORTDIALOG_H

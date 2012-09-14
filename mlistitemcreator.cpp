#include "mlistitemcreator.h"
#include <QDebug>

void MListItemCreator::updateCell(const QModelIndex& index, MWidget * cell) const
{
    MBasicListItem * contentItem = qobject_cast<MBasicListItem *>(cell);
    contentItem->setObjectName("CommonBasicListItem");
    contentItem->setItemStyle(MBasicListItem::TitleWithSubtitle);

    QVariant data = index.data(Qt::DisplayRole);
    QStringList rowData = data.value<QStringList>();
    QString title = rowData[0];

    title.replace(QRegExp("<[^>]*/p>"), QString('\n'));
    title.remove(QRegExp("<[^>]*>"));
    title.remove("p, li { white-space: pre-wrap; }", Qt::CaseSensitive);

    // Get only the first line from the title
    QStringList titleList = title.split(QString('\n'));

    for (int i = 0; i < titleList.length(); i++) {
        QString newTitle = titleList[i];
        newTitle.remove(QString('\n'));

        if (!newTitle.isEmpty()) {
            title = newTitle;
            break;
        }
    }

    // Check if first char is an empty space
    if (title.at(0) == ' ' || title.at(0) == '\n') {
        title.remove(0, 1);
    }

    contentItem->setTitle(title);
    contentItem->setSubtitle(rowData[1]);
}

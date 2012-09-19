#include "mlistitemcreator.h"
#include <QDebug>

void MListItemCreator::updateCell(const QModelIndex& index, MWidget * cell) const
{
    MBasicListItem * contentItem = qobject_cast<MBasicListItem *>(cell);
    contentItem->setObjectName("CommonBasicListItem");
    contentItem->setItemStyle(MBasicListItem::TitleWithSubtitle);

    QVariant data = index.data(Qt::DisplayRole);
    QStringList rowData = data.value<QStringList>();

    contentItem->setTitle(rowData[0]);
    contentItem->setSubtitle(rowData[1]);
}

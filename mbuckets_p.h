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

#ifndef MBUCKETS_P_H
#define MBUCKETS_P_H

#include <QStringList>
#include <QVector>
#include <QDate>


class MBuckets;

class MBucketsPrivate
{
    Q_DECLARE_PUBLIC(MBuckets)

    MBucketsPrivate();
    void copy(const MBuckets &other);

    void setItems(const QStringList &items, Qt::SortOrder sortOrder);
    void clear();
    bool removeBucketItems(int bucketIndex, int itemIndex, int count);
    void removeEmptyBucket(int bucketIndex);

    //
    // Data members
    //

    QStringList allBuckets;
    QStringList buckets; // used buckets
    QVector<QStringList> bucketItems;
    // Intentionally not using QList to avoid flattening the list
    // when trying to append another QStringList
    QVector<QList<int> > origIndices;

    MBuckets *q_ptr;
};


// Helper class to retain the original index for items during sorting
struct MBucketsItem
{
    QString text;
    int origIndex;

    MBucketsItem(QString text, int origIndex) :
        text(text), origIndex(origIndex) {}
};



// Functor for qStableSort() comparison
class MBucketsItemComparator
{
public:
    MBucketsItemComparator(Qt::SortOrder sortOrder = Qt::AscendingOrder):
        sortOrder(sortOrder)
        {
        }

    bool operator()(const MBucketsItem &left, const MBucketsItem &right)
    {
        return sortOrder == Qt::DescendingOrder ?
                    (QDate::fromString(right.text) < QDate::fromString(left.text)) :
                    (QDate::fromString(left.text) < QDate::fromString(right.text));
    }

private:
    Qt::SortOrder sortOrder;
};


#endif // MBUCKETS_P_H

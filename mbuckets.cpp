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

#include "mbuckets.h"
#include "mbuckets_p.h"


MBucketsPrivate::MBucketsPrivate() :
    q_ptr(0)
{
}

void MBucketsPrivate::setItems(const QStringList &unsortedItems, Qt::SortOrder sortOrder)
{
    // Remember to call clear() first if this is called from somewhere else than a constructor!
    QList<MBucketsItem> items;

    for (int i = 0; i < unsortedItems.size(); ++i) {
        items.append(MBucketsItem(unsortedItems.at(i), i));
    }
    qStableSort(items.begin(), items.end(), MBucketsItemComparator(sortOrder));

    QString lastBucket;
    QStringList lastBucketItems;
    QList<int>  lastBucketOrigIndices;

    foreach (const MBucketsItem &item, items) {
        // Use the whole string
        QString bucket = item.text.isEmpty() ? "" : item.text;

        if (bucket != lastBucket) {
            if (!lastBucketItems.isEmpty()) {
                // Found a new bucket - store away the old one
                buckets << lastBucket;
                bucketItems << lastBucketItems;
                origIndices << lastBucketOrigIndices;
                lastBucketItems.clear();
                lastBucketOrigIndices.clear();
            }
            lastBucket = bucket;
        }
        lastBucketItems << item.text;
        lastBucketOrigIndices << item.origIndex;
    }

    if (!lastBucketItems.isEmpty()) {
        buckets << lastBucket;
        bucketItems << lastBucketItems;
        origIndices << lastBucketOrigIndices;
    }
}

void MBucketsPrivate::clear()
{
    buckets.clear();
    bucketItems.clear();
    origIndices.clear();
}

bool MBucketsPrivate::removeBucketItems(int bucketIndex, int itemIndex, int count)
{
    if (bucketIndex < 0 || bucketIndex >= bucketItems.count() || itemIndex < 0 || count <= 0)
        return false;

    QStringList &itemList = bucketItems[bucketIndex];

    if (itemIndex + count > itemList.count())
        return false;

    for (int i = 0; i < count; ++i) {
        QList<int> &origIndexList = origIndices[bucketIndex];
        int removedOrigIndex = origIndexList.at(itemIndex);
        origIndexList.removeAt(itemIndex);
        itemList.removeAt(itemIndex);

        // Decrement original index of all items after the removed item

        for (int j = 0; j < origIndices.count(); ++j) {
            QList<int> &origIndexList = origIndices[j];
            for (int k = 0; k < origIndexList.count(); ++k) {
                if (origIndexList.at(k) > removedOrigIndex) {
                    --origIndexList[k];
                }
            }
        }
    }

    return itemList.isEmpty();
}

void MBucketsPrivate::removeEmptyBucket(int bucketIndex)
{
    if (bucketIndex >= 0 && bucketIndex < bucketItems.count() &&
        bucketItems.at(bucketIndex).isEmpty()) {
        buckets.removeAt(bucketIndex);
        bucketItems.remove(bucketIndex);
        origIndices.remove(bucketIndex);
    }
}

void MBucketsPrivate::copy(const MBuckets &other)
{
    allBuckets  = other.d_func()->allBuckets;
    bucketItems = other.d_func()->bucketItems;
    buckets     = other.d_func()->buckets;
    origIndices = other.d_func()->origIndices;
}


MBuckets::MBuckets():
    d_ptr(new MBucketsPrivate())
{
    Q_D(MBuckets);

    d->q_ptr = this;
}

MBuckets::MBuckets(const MBuckets &other)
    : d_ptr(new MBucketsPrivate)
{
    Q_D(MBuckets);

    d->q_ptr = this;
    d->copy(other);
}

MBuckets::MBuckets(const QStringList &unsortedItems, Qt::SortOrder sortOrder) :
    d_ptr(new MBucketsPrivate())
{
    Q_D(MBuckets);

    d->q_ptr = this;
    d->setItems(unsortedItems, sortOrder);
}

MBuckets &MBuckets::operator=(const MBuckets &other)
{
    Q_D(MBuckets);

    d->copy(other);
    return *this;
}

MBuckets::~MBuckets()
{
    Q_D(MBuckets);

    delete d;
}

void MBuckets::setItems(const QStringList &items, Qt::SortOrder sortOrder)
{
    Q_D(MBuckets);

    d->clear();
    d->setItems(items, sortOrder);
}

int MBuckets::bucketCount() const
{
    Q_D(const MBuckets);

    return d->buckets.count();
}

QString MBuckets::bucketName(int bucketIndex) const
{
    Q_D(const MBuckets);

    if (bucketIndex < 0 || bucketIndex >= d->buckets.size())
        return QString();
    else
        return d->buckets.at(bucketIndex);
}

QString MBuckets::bucketName(const QString &item) const
{
    return item.isEmpty() ? "" : QString(item[0]);
}

int MBuckets::bucketIndex(const QString &bucketName) const
{
    Q_D(const MBuckets);

    return d->buckets.indexOf(bucketName);
}

QStringList MBuckets::bucketContent(int bucketIndex) const
{
    Q_D(const MBuckets);

    if (bucketIndex < 0 || bucketIndex >= d->buckets.size())
        return QStringList();
    else
        return d->bucketItems.at(bucketIndex);
}

int MBuckets::origItemIndex(int bucketIndex, int indexInBucket) const
{
    Q_D(const MBuckets);

    if (bucketIndex >= 0 && bucketIndex < d->buckets.size()) {
        const QList<int> &origIndices = d->origIndices.at(bucketIndex);
        if (indexInBucket >= 0 && indexInBucket < origIndices.size()) {
            return origIndices.at(indexInBucket);
        }
    }
    return -1;
}

int MBuckets::bucketSize(int bucketIndex) const
{
    Q_D(const MBuckets);

    if (bucketIndex < 0 || bucketIndex >= d->buckets.size())
        return -1;
    else
        return d->bucketItems.at(bucketIndex).size();
}

bool MBuckets::isEmpty() const
{
    Q_D(const MBuckets);

    return d->bucketItems.isEmpty();
}

void MBuckets::clear()
{
    Q_D(MBuckets);

    d->clear();
}

bool MBuckets::removeBucketItems(int bucketIndex, int itemIndex, int count)
{
    Q_D(MBuckets);

    return d->removeBucketItems(bucketIndex, itemIndex, count);
}

void MBuckets::removeEmptyBucket(int bucketIndex)
{
    Q_D(MBuckets);

    return d->removeEmptyBucket(bucketIndex);
}

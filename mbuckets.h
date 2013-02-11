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

#ifndef MBuckets_H
#define MBuckets_H

#include <QStringList>

class MBucketsPrivate;

class MBuckets
{
public:
    /*!
     * \brief Constructor: Create an empty MBuckets object with the
     * current date.
     *
     * Remember to call setItems() at some point.
     */
    MBuckets();

    /*!
     * \brief Constructor: Copies date buckets from the other reference.
     */
    MBuckets(const MBuckets &other);

    /*!
     * \brief Constructor: Create an MBuckets object from the specified
     * list of items with the current locale.
     *
     * This is equivalent to creating an empty MBuckets object and
     * calling setItems() with that item list.
     */
    explicit MBuckets(const QStringList &unsortedItems, Qt::SortOrder sortOrder = Qt::AscendingOrder);

    /*!
     * \brief Destructor.
     */
    virtual ~MBuckets();

    /*!
     * \brief Set the items for this MBuckets object and recalculate all
     * values.
     *
     * This sorts all items into their corresponding bucket according to locale
     * rules.
     *
     * Each bucket has an index (starting with 0), a name and a number of
     * items. Both the buckets and the items inside each bucket are sorted in
     * the specified sort order according to locale rules.
     *
     * Only buckets that actually contain items are taken into account. If
     * there was no item starting with "X" in the item list specified in the
     * constructor, there will not be a bucket "X".
     *
     * If there were any previous items or buckets, this function removes them
     * first.
     *
     * Since the items list is sorted internally anyway, there is no benefit in
     * sorting them first before passing to this function.
     */
    void setItems(const QStringList &unsortedItems, Qt::SortOrder sortOrder = Qt::AscendingOrder);

    /*!
     * \brief Return the number of buckets.
     */
    int bucketCount() const;

    /*!
     * \brief Return the name of the bucket with the specified index (starting
     * with 0).
     */
    QString bucketName(int bucketIndex) const;

    /*!
     * \brief Return the name of the bucket for the specified item.
     */
    QString bucketName(const QString &item) const;

    /*!
     * \brief Return the index of the specified bucket name or -1 if there is
     * no bucket with that name.
     */
    int bucketIndex(const QString &bucketName) const;

    /*!
     * \brief Return the bucket content for the specified bucket index.
     */
    QStringList bucketContent(int bucketIndex) const;

    /*!
     * \brief Return the original index of an item
     *
     * This returns the original index from the list passed in the constructor
     * of an item that is now sorted into bucket no. 'bucketIndex'. See also
     * the example in the intro of this class.
     *
     * If there is no item at that position, -1 is returned.
     *
     * If during the life time of this MBuckets object any items are
     * removed with removeBucketItems(), the original index of each item that
     * came after the removed item is decremented accordingly, just as if the
     * item was removed from the original items list set with setItems() or in
     * the constructor.
     *
     * \param bucketIndex index of the bucket.
     * \param indexInBucket index within the bucket. Each bucket starts with 0.
     */
    int origItemIndex(int bucketIndex, int indexInBucket) const;

    /*!
     * \brief Return the bucket size for the specified bucket index, i.e. the
     * number of items in that bucket.
     *
     * This returns -1 if there is no bucket with that index.
     */
    int bucketSize(int bucketIndex) const;

    /*!
     * \brief Check if there are any buckets or items in this MBuckets
     * object.
     */
    bool isEmpty() const;

    /*!
     * \brief Clear this MBuckets object: Remove all items and all
     * buckets.
     *
     * This is equivalent to setting an empty items list.
     */
    void clear();

    /*!
     * \brief Remove a range of items from bucket.
     *
     * This removes 'count' items from the bucket 'bucketIndex', starting with
     * index 'indexInBucket' inside that bucket.
     *
     * This returns 'true' if the bucket is empty afterwards (i.e., it should
     * be removed), 'false' otherwise.
     *
     * The original indices of items (origItemIndex()) will be decremented
     * accordingly, just as if the items were removed from the QStringList
     * passed in the constructor or in setItems().
     */
    bool removeBucketItems(int bucketIndex, int indexInBucket, int count=1);

    /*!
     * \brief Remove a bucket if it is empty.
     *
     * This should be done after removeBucketItems() returned true. The
     * rationale why removeBucketItems() doesn't do this automatically is
     * because SortFilterProxyModels that are attached to a model that uses
     * MBuckets will get very confused if toplevel items disappear
     * because the items below them were removed.
     *
     * If the bucket is not empty, this function does nothing.
     */
    void removeEmptyBucket(int bucketIndex);

    /*!
     * \brief Copies buckets and bucket items from the other reference.
     */
    MBuckets& operator =(const MBuckets &other);

private:
    Q_DECLARE_PRIVATE(MBuckets)

    MBucketsPrivate *const d_ptr;
};

#endif // MBUCKETS_H

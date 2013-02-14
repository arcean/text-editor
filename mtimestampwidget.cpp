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

#include <QDateTime>

#include "mtimestampwidget.h"

MTimeStampWidget::MTimeStampWidget(QGraphicsItem *parent)
    : MWidgetController(parent)
{
    this->setObjectName("NoteEditTimeStampArea");
    mainLayout = new QGraphicsLinearLayout(Qt::Vertical, this);
    mainLayout->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    timeStampLabel = new MLabel();
    timeStampLabel->setObjectName("NoteEditTimeStampLabel");
    mainLayout->addItem(timeStampLabel);
}

MTimeStampWidget::~MTimeStampWidget()
{

}

void MTimeStampWidget::setAutoDateTime()
{
    QDateTime date = QDateTime::currentDateTime();
    QString dateString = date.toString("d MMM yyyy | h:mm");

    timeStampLabel->setText(dateString);
}

void MTimeStampWidget::setDateTime(const QString &text)
{
    timeStampLabel->setText(text);
}

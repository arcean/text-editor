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

#ifndef MTIMESTAMPWIDGET_H
#define MTIMESTAMPWIDGET_H

#include <MWidgetController>
#include <MLabel>
#include <QObject>
#include <QGraphicsLinearLayout>

class MTimeStampWidget : public MWidgetController
{
    Q_OBJECT

public:
    MTimeStampWidget(QGraphicsItem *parent = 0);
    ~MTimeStampWidget();

    void setAutoDateTime();
    void setDateTime(const QString &text);

public slots:

private slots:

signals:

private:
    QGraphicsLinearLayout *mainLayout;

    MLabel *timeStampLabel;

};

#endif // MTIMESTAMPWIDGET_H

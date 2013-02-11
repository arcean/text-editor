/***************************************************************************
**
** Copyright (C) 2012, 2013 Tomasz Pieniążek
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

#ifndef CLICKABLEVIEWHEADER_H
#define CLICKABLEVIEWHEADER_H

#include <MImageWidget>
#include <QGraphicsLinearLayout>
#include <MLabel>
#include <MButton>

class ClickableViewHeader : public MButton
{
public:
    ClickableViewHeader(QGraphicsItem *parent = 0) :
        MButton(parent),
        linearLayout(0),
        titleWidget(0)
    {
        setObjectName("CommonHeaderPanel");
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        setContentsMargins(0, 0, 0, 0);
        linearLayout = new QGraphicsLinearLayout(Qt::Horizontal, this);
        linearLayout->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        // Set left margin to 8, margin needed by selectorImage
        linearLayout->setContentsMargins(0, 0, 8, 0);
        linearLayout->setSpacing(0);
        titleWidget = new MLabel();
        titleWidget->setTextElide(true);
        titleWidget->setStyleName("CommonHeaderInverted");
        selectorImage = new MImageWidget();
        selectorImage->setStyleName("SelectorImage");

        linearLayout->addItem(titleWidget);
        linearLayout->addItem(selectorImage);
        linearLayout->setAlignment(selectorImage, Qt::AlignVCenter);
    }

    void setTitle(const QString &title)
    {
        titleWidget->setText(title);
    }

private:
    QGraphicsLinearLayout *linearLayout;
    MLabel *titleWidget;
    MImageWidget *selectorImage;

};

#endif // CLICKABLEVIEWHEADER_H

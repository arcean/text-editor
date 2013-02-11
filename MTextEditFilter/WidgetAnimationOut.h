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

#ifndef WIDGETANIMATIONOUT_H
#define WIDGETANIMATIONOUT_H

#include <QObject>
#include <MAnimation>
#include <QGraphicsItem>

class WidgetAnimationOut : public MAnimation {
    Q_OBJECT
    //M_ANIMATION(FadeInAnimationStyle)

  public:
    WidgetAnimationOut(QGraphicsItem *item, QObject *parent = NULL) : MAnimation(parent),mItem(item) {};
    virtual void updateCurrentTime (int currentTime ) {
       mItem->setOpacity(50.0/currentTime);
    }
    virtual int duration () const { return 50; }
  protected:
    QGraphicsItem *mItem;
};

#endif // WIDGETANIMATIONOUT_H

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

#ifndef WIDGETANIMATIONIN_H
#define WIDGETANIMATIONIN_H

#include <QObject>
#include <MAnimation>
#include <QGraphicsItem>

class WidgetAnimationIn : public MAnimation {
    Q_OBJECT
 //   M_ANIMATION(FadeInAnimationStyle)

  public:
    WidgetAnimationIn(QGraphicsItem *item, QObject *parent = NULL) : MAnimation(parent),mItem(item) {};
    virtual void updateCurrentTime (int currentTime ) {
       mItem->setOpacity(currentTime/500.0);
    }
    virtual int duration () const { return 500; }
  protected:
    QGraphicsItem *mItem;
};

#endif // WIDGETANIMATIONIN_H

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

#include <QSettings>

#include "settings.h"

Settings::Settings(QObject *parent) :
    QObject(parent)
{
}
int Settings::getSortMode()
{
    QSettings settings;
    int value =  settings.value("common/sortMode", 0).toInt();

    return value;
}

void Settings::setSortMode(int sortMode)
{
    QSettings settings;

    settings.setValue("common/sortMode", sortMode);
}


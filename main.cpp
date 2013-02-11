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

#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationPage>
#include <MButton>
#include <MToolBar>
#include <MAction>
#include <MWidgetAction>
#include <MTextEdit>
#include <QObject>
#include <MExport>
#include <MComponentCache>

#include "mainpage.h"

M_EXPORT int main(int argc, char *argv[]){
    MApplication* application = MComponentCache::mApplication(argc, argv);
    application->setOrganizationName("arcean");
    application->setOrganizationDomain("arcean.com");
    application->setApplicationName("exnote");

    MApplicationWindow* window = MComponentCache::mApplicationWindow();
    MainPage page;

    page.appear(window);

    window->show();

    return application->exec();
 }

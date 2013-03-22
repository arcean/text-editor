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

#ifndef SHARECOMMAND_H
#define SHARECOMMAND_H

#include <QObject>

class ShareCommand : public QObject
{
    Q_OBJECT
public:
    explicit ShareCommand(QObject *parent = 0);

    void share(const QString &filePath);
    void shareAsText(const QString &text, const QString &title);
    
signals:
    
public slots:

private:

};

#endif // SHARECOMMAND_H

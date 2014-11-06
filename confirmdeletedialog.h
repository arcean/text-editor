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

#ifndef CONFIRMDELETEDIALOG_H
#define CONFIRMDELETEDIALOG_H

#include <QObject>
#include <MMessageBox>

class ConfirmDeleteDialog : public MMessageBox
{
    Q_OBJECT
public:
    explicit ConfirmDeleteDialog(const QString &fileName);
    
signals:
    
public slots:

private:
    QString m_fileName;
    
};

#endif // CONFIRMDELETEDIALOG_H

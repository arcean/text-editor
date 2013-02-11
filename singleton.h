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

#ifndef SINGLETON_H
#define SINGLETON_H

#include <QObject>

template <class T>
class Singleton
{
public:
    static T& Instance()
    {
        static T _instance;                     // create static instance of our class
        return _instance;                       // return it
    }

private:
    Singleton();                                // hide constructor
    ~Singleton();                               // hide destructor
    Singleton(const Singleton &);               // hide copy constructor
    Singleton& operator=(const Singleton &);    // hide assign op
};

#endif // SINGLETON_H

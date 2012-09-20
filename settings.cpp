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


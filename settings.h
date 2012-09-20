#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);

    int getSortMode();
    void setSortMode(int sortMode);
};

#endif // SETTINGS_H

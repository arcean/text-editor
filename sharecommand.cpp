#include <QString>
#include <MDataUri>
#include <QDebug>
#include <maemo-meegotouch-interfaces/shareuiinterface.h>
#include <QFile>

#include "sharecommand.h"

ShareCommand::ShareCommand(QObject *parent) :
    QObject(parent)
{
}

void ShareCommand::share(QString filePath)
{
    QString targetName;

    if (filePath.at(0) == QChar('f')) {
        targetName = filePath;
        targetName.replace(0,7,"");
    }
    else
        targetName = filePath;

    QFileInfo shareFileInfo(targetName);
    QString shareFileURL = shareFileInfo.canonicalFilePath();

    if (shareFileURL.isEmpty()) {
        qCritical() << "Empty sharing file URL...";
        return;
    }

    QStringList items;
    items << shareFileURL;

    ShareUiInterface shareIf("com.nokia.ShareUi");

    if (shareIf.isValid()) {
        shareIf.share(items);
    }
    else
        qCritical() << "Invalid ShareUi";

}

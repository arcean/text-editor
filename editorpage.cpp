#include <MLayout>
#include <MPannableViewport>
#include <QGraphicsLinearLayout>
#include <MLinearLayoutPolicy>
#include <MLabel>
#include <MRichTextEdit>
#include <MAction>
#include <QDateTime>
#include <QDir>
#include <QDebug>

#include "editorpage.h"
#include "viewHeader.h"

EditorPage::EditorPage(QGraphicsItem *parent)
{
    Q_UNUSED(parent);

    /* Sheet header */
    MBasicSheetHeader *sheetHeader = new MBasicSheetHeader;

    saveAction = new QAction("Save", sheetHeader);
    sheetHeader->setPositiveAction(saveAction);
    sheetHeader->setNegativeAction(new QAction("Cancel", sheetHeader));

    setHeaderWidget(sheetHeader);

    // Create main app viewport
    MPannableViewport *viewportWidget = new MPannableViewport;

    this->setCentralWidget(viewportWidget);
    /////////////////////////////////////////////////// ACTIONS

    /////////////////////////////////////////////////// CONTENT
    editor = new MRichTextEdit(MTextEditModel::MultiLine);

    viewportWidget->setWidget(editor);
    /////////////////////////////////////////////////// SIGNALS
    connect(sheetHeader->negativeAction(), SIGNAL(triggered()), SLOT(processDialogRejected()));
    connect(sheetHeader->positiveAction(), SIGNAL(triggered()), SLOT(processDialogAccepted()));
    connect(editor, SIGNAL(textChanged()), this, SLOT(enableSaveButton()));

    /////////////////////////////////////////////////// OTHER
    // Disable "Save" button
    saveAction->setDisabled(true);
}

void EditorPage::setFocusOnEditor()
{
    // Focus on the editor
    editor->setFocus(Qt::OtherFocusReason);
}

void EditorPage::enableSaveButton()
{
    bool disable = editor->text().length() > 0 ? false : true;

    saveAction->setDisabled(disable);
}

void EditorPage::processDialogRejected()
{
    this->close();
}

void EditorPage::processDialogAccepted()
{
    if (editor->text().length() > 0)
        writeToFile();

    this->close();
}

void EditorPage::loadFile(const QString& filePath, int currentRow, int parentRow)
{
    QFile file (filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "[E]: could not open the file" << filePath;
        return;
    }

    QTextStream in(&file);
    QString line = in.readLine();

    while (!in.atEnd()) {
        line += in.readLine();
       // line += "\n";
    }

    editor->setHtml(line);
    this->fileName = filePath;
    this->currentRow = currentRow;
    this->parentRow = parentRow;
}

void EditorPage::writeToFile()
{
    QString name;

    if (fileName.length() <= 0)
        name = "/home/user/MyDocs/exnote/" + getNewFilename();
    else
        name = fileName;

    QFile file(name);
    file.remove();

    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << editor->toHtml();
    file.close();

    if (fileName.length() <= 0)
        emit reloadModel(-1, -1);
    else
        emit reloadModel(currentRow, parentRow);
}

QString EditorPage::getNewFilename()
{
    QDir appDir("/home/user/MyDocs/exnote/");
    bool ready = false;
    int counter = 0;
    QDateTime time = QDateTime::currentDateTime();
    QString baseName = time.toString("dd_mm_yy");

    if(!appDir.exists()) {
        appDir.mkdir("/home/user/MyDocs/exnote/");
    }

    QDir::setCurrent("/home/user/MyDocs/exnote/");
    QFile file;

    while (!ready) {
        if(counter > 0)
            file.setFileName(baseName + "(" + QString::number(counter) +").html");
        else
            file.setFileName(baseName + ".html");

        if(!file.exists()) {
            return file.fileName();
        }

        counter++;

        if(counter > 500000)
            ready = true;
    }

    qDebug() << "New filename" << file.fileName();
    return file.fileName();
}

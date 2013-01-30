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

EditorPage::EditorPage(bool focusOnEditor, QGraphicsItem *parent)
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

    if (focusOnEditor)
        connect(this, SIGNAL(appeared()), this, SLOT(setFocusOnEditor()));

    /////////////////////////////////////////////////// OTHER
    // Disable "Save" button
    saveAction->setDisabled(true);

    // Initialize Utils
    utils = &Singleton<Utils>::Instance();
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

    if (fileName.length() <= 0) {
        QString text = editor->text();

        if (text.length() > 32)
            text.chop(text.length() - 32);

        QString generatedFilename = utils->getNewFilename(text);

        name = "/home/user/MyDocs/exnote/" + generatedFilename;
    }
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

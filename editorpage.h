#ifndef EDITORPAGE_H
#define EDITORPAGE_H

#include <QObject>
#include <MSheet>
#include <MRichTextEdit>
#include <MBasicSheetHeader>

class EditorPage : public MSheet
{
    Q_OBJECT
public:
    explicit EditorPage(bool focusOnEditor = false, QGraphicsItem *parent = 0);

    void loadFile(const QString& filePath, int currentRow, int parentRow);

signals:
    void reloadModel(int oldRow, int oldParentRow);
    
public slots:

private slots:
    void processDialogRejected();
    void processDialogAccepted();
    void enableSaveButton();
    void setFocusOnEditor();

private:
    QString getNewFilename();
    void writeToFile();

    QAction *saveAction;
    MRichTextEdit *editor;
    // Current filename
    QString fileName;
    int currentRow;
    int parentRow;
    
};

#endif // EDITORPAGE_H

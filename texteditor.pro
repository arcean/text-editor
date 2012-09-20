#-------------------------------------------------
#
# Project created by QtCreator 2011-09-08T15:25:41
#
#-------------------------------------------------

QT       += core gui

TARGET = exnote
TEMPLATE = app


SOURCES += main.cpp \
    mainpage.cpp \
    filemodel.cpp \
    cell.cpp \
    editorpage.cpp \
    mlistitemcreator.cpp \
    aboutdialog.cpp \
    confirmdeletedialog.cpp \
    entry.cpp

HEADERS  += \
    mainpage.h \
    mainpage.h \
    filemodel.h \
    cell.h \
    editorpage.h \
    viewHeader.h \
    mlistitemcreator.h \
    aboutdialog.h \
    confirmdeletedialog.h \
    entry.h \
    ClickableViewHeader.h

FORMS    +=

CONFIG += meegotouch
CONFIG += meegotouch-boostable

symbian {
    TARGET.UID3 = 0xe8f85d27
    # TARGET.CAPABILITY +=
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}

contains(MEEGO_EDITION,harmattan) {
    target.path = /opt/exnote/bin
    INSTALLS += target

    icon.path = /usr/share/icons/hicolor/80x80/apps
    icon.files = data/exnote.png

    desktop.path = /usr/share/applications
    desktop.files = data/exnote.desktop

    css.path = /opt/exnote/style/
    css.files = data/exnote.css

    gfx.path = /opt/exnote/data/
    gfx.files = data/dialog-question.png

    INSTALLS += icon desktop css gfx

}

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    data/texteditor.png

RESOURCES +=

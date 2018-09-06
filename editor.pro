#-------------------------------------------------
#
# Project created by QtCreator 2015-02-21T20:48:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = editor
TEMPLATE = app

TRANSLATIONS += translations/multilanguage_zh.ts \
                translations/multilanguage_en.ts


SOURCES += main.cpp\
        mainwindow.cpp \
    button.cpp \
    calculator.cpp \
    highlighter.cpp \
    codeeditor.cpp \
    parser.cpp \
    musicplayer.cpp

HEADERS  += mainwindow.h \
    button.h \
    calculator.h \
    highlighter.h \
    codeeditor.h \
    parser.h \
    udef.h \
    musicplayer.h

RESOURCES += \
    graphics.qrc \
    config.qrc \
    translations.qrc

RC_FILE += \
    editor.rc

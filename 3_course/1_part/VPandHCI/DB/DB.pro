#-------------------------------------------------
#
# Project created by QtCreator 2017-12-10T14:51:40
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DB
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    database.cpp \
    userdata.cpp \
    dialog.cpp \
    dialoguserabon.cpp

HEADERS  += mainwindow.h \
    database.h \
    userdata.h \
    dialog.h \
    dialoguserabon.h

FORMS    += mainwindow.ui \
    userdata.ui \
    dialog.ui \
    dialoguserabon.ui

RESOURCES += \
    res.qrc

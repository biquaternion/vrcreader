#-------------------------------------------------
#
# Project created by QtCreator 2014-10-15T12:32:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vrcReader
TEMPLATE = app

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
        src/vrcreader.cpp \
        src/vrcplayer.cpp

HEADERS  += include/mainwindow.h \
            include/vrcreader.h \
            include/vrcplayer.h

FORMS    += src/mainwindow.ui

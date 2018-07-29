#-------------------------------------------------
#
# Project created by QtCreator 2014-10-15T12:32:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vrcReader
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    vrcreader.cpp \
    vrcplayer.cpp

HEADERS  += mainwindow.h \
    vrcreader.h \
    vrcplayer.h

FORMS    += mainwindow.ui

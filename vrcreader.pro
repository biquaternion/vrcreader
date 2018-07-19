#-------------------------------------------------
#
# Project created by QtCreator 2014-10-15T12:32:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vrcReader
TEMPLATE = app

CONFIG -= use_mingw
CONFIG -= use_msvc
CONFIG += use_linux

SOURCES += main.cpp\
        mainwindow.cpp \
    vrc.cpp

HEADERS  += mainwindow.h \
#    FrameIterator.h \
    vrc.h

FORMS    += mainwindow.ui


#INCLUDEPATH += C:\OpenCV__\246\opencv\pro\install\include

use_linux {
LIBS += -L/usr/local/lib \
        -lopencv_core \
        -lopencv_highgui \
        -lopencv_imgcodecs \
        -lopencv_imgproc
INCLUDEPATH += /usr/local/include
}

use_mingw {
LIBS     += -LC:\OpenCV__\246\opencv\pro\install\lib \
            -lopencv_calib3d246 \
            -lopencv_contrib246 \
            -lopencv_core246 \
            -lopencv_features2d246 \
            -lopencv_flann246 \
            -lopencv_gpu246 \
            -lopencv_highgui246 \
            -lopencv_imgproc246 \
            -lopencv_legacy246 \
            -lopencv_ml246 \
            -lopencv_nonfree246 \
            -lopencv_objdetect246 \
            -lopencv_photo246 \
            -lopencv_stitching246 \
            -lopencv_superres246 \
            -lopencv_ts246 \
            -lopencv_video246 \
            -lopencv_videostab246
}
use_msvc {
LIBS     += -LC:\OpenCV__\246\opencv\build\x86\vc9\lib \
            -lopencv_calib3d246 \
            -lopencv_contrib246 \
            -lopencv_core246 \
            -lopencv_features2d246 \
            -lopencv_flann246 \
            -lopencv_gpu246 \
            -lopencv_highgui246 \
            -lopencv_imgproc246 \
            -lopencv_legacy246 \
            -lopencv_ml246 \
            -lopencv_nonfree246 \
            -lopencv_objdetect246 \
            -lopencv_photo246 \
            -lopencv_stitching246 \
            -lopencv_superres246 \
            -lopencv_ts246 \
            -lopencv_video246 \
            -lopencv_videostab246
}

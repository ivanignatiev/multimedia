#-------------------------------------------------
#
# Project created by QtCreator 2014-06-07T15:14:04
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = multimedia
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    videoinputcontrolthread.cpp \
    videorecorder.cpp \
    videodevice.cpp \
    videoframe.cpp \
    qglcanvas.cpp \
    videoplayer.cpp \
    videolibrary.cpp \
    audiorecorder.cpp \
    audiodevice.cpp \
    ioutputvideostream.cpp \
    iinputvideostream.cpp \
    fileoutputvideostream.cpp \
    videoencoder.cpp \
    fileinputvideostream.cpp \
    videodecoder.cpp

HEADERS  += mainwindow.h \
    videoinputcontrolthread.h \
    videorecorder.h \
    videodevice.h \
    videoframe.h \
    config.h \
    qglcanvas.h \
    videoplayer.h \
    videolibrary.h \
    audiorecorder.h \
    audiodevice.h \
    ioutputvideostream.h \
    iinputvideostream.h \
    fileoutputvideostream.h \
    videoencoder.h \
    fileinputvideostream.h \
    videodecoder.h

FORMS    += \
    mainwindow.ui

OTHER_FILES += \
    README.md

INCLUDEPATH += /usr/include/opencv

LIBS += /usr/lib/x86_64-linux-gnu/libopencv_core.so \
    /usr/lib/x86_64-linux-gnu/libopencv_highgui.so

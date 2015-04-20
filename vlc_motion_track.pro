#-------------------------------------------------
#
# Project created by QtCreator 2015-04-17T20:42:05
#
#-------------------------------------------------

QT       += core gui

CONFIG += link_pkgconfig
PKGCONFIG += opencv

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cam_motion_track
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    vlcreader.cpp \
    videoframe.cpp \
    motiontracker.cpp \
    motiontrackerdialog.cpp

HEADERS  += mainwindow.h \
    vlcreader.h \
    videoframe.h \
    motiontracker.h \
    motiontrackerdialog.h \
    motiontracker_type.h

FORMS    += mainwindow.ui \
    motiontrackerdialog.ui

LIBS += -lvlc

#-------------------------------------------------
#
# Project created by QtCreator 2017-01-18T01:46:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cg_lab2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    myimage.cpp \
    histogramWindow.cpp

HEADERS  += mainwindow.h \
    MyImage.h \
    histogramWindow.h

FORMS    += mainwindow.ui \
    histogramWindow.ui

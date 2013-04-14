#-------------------------------------------------
#
# Project created by QtCreator 2013-04-14T09:40:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Barcode
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ean13.cpp \
    drawwidget.cpp

HEADERS  += mainwindow.h \
    ean13.h \
    drawwidget.h

FORMS    += mainwindow.ui \
    drawwidget.ui

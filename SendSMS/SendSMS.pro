#-------------------------------------------------
#
# Project created by QtCreator 2013-04-11T19:37:33
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SendSMS
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mainsmshandler.cpp \
    abstractsmsgateway.cpp

HEADERS  += mainwindow.h \
    mainsmshandler.h \
    abstractsmsgateway.h

FORMS    += mainwindow.ui

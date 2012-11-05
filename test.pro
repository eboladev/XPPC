#-------------------------------------------------
#
# Project created by QtCreator 2012-10-20T11:57:57
#
#-------------------------------------------------

QT       += core gui network sql

TARGET = test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    receiptmanager.cpp \
    joblistonreceiptdialog.cpp \
    connectdialog.cpp \
    sqlextension.cpp \
    setupmanager.cpp

HEADERS  += mainwindow.h \
    receiptmanager.h \
    joblistonreceiptdialog.h \
    connectdialog.h \
    sqlextension.h \
    setupmanager.h

FORMS    += mainwindow.ui \
    receiptmanager.ui \
    joblistonreceiptdialog.ui \
    connectdialog.ui
#include(./ncreport_library.pri)
INCLUDEPATH += "./ncreport/include"
DEPENDPATH += "./ncreport/lib"
LIBS += -L"./ncreport/lib/" -lncreport2

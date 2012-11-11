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
    setupmanager.cpp \
    branchwidget.cpp \
    closeticketwidget.cpp

HEADERS  += mainwindow.h \
    receiptmanager.h \
    joblistonreceiptdialog.h \
    connectdialog.h \
    sqlextension.h \
    setupmanager.h \
    branchwidget.h \
    closeticketwidget.h

FORMS    += mainwindow.ui \
    receiptmanager.ui \
    joblistonreceiptdialog.ui \
    connectdialog.ui \
    branchwidget.ui \
    closeticketwidget.ui

DEFINES += NCREPORT_IMPORT
win32-g++ {
    INCLUDEPATH += -L$$PWD/../ncreport/include
    CONFIG( debug, debug|release ) {
        LIBS += $$PWD/ncreport/lib/libncreportd2.a
    } else {
        LIBS += $$PWD/ncreport/lib/libncreport2.a
    }

}

OTHER_FILES += \
    report.xml

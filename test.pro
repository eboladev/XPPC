#-------------------------------------------------
#
# Project created by QtCreator 2012-10-20T11:57:57
#
#-------------------------------------------------

QT       += core gui network sql

RCC_DIR = $$PWD/build/xppc/rcc
UI_DIR = $$PWD/build/xppc/ui
MOC_DIR = $$PWD/build/xppc/moc
DESTDIR = $$PWD/app

CONFIG(debug, debug|release):{
DEFINES+=DEBUG
OBJECTS_DIR = $$PWD/build/xppc/debug/obj
} else: {
DEFINES+=RELEASE
OBJECTS_DIR = $$PWD/build/xppc/release/obj
}

TARGET = xppc
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

win32-g++ {

    INCLUDEPATH += ./ncreport/include

    CONFIG( debug, debug|release ) {
	LIBS += "./ncreport/lib/libncreportd2.a"
    } else {
        LIBS += "./ncreport/lib/libncreport2.a"
    }

}

OTHER_FILES += \
    report.xml

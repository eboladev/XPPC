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

INCLUDEPATH += $$PWD

SOURCES += main.cpp\
        mainwindow.cpp \
    receiptmanager.cpp \
    joblistonreceiptdialog.cpp \
    connectdialog.cpp \
    sqlextension.cpp \
    setupmanager.cpp \
    branchwidget.cpp \
    changeuserdialog.cpp \
    usermanagementdialog.cpp \
    filterlineedit.cpp \
    fancylineedit.cpp \
    userloginpassmanager.cpp \
    userstatisticwidget.cpp \
    productcategorymanager.cpp \
    customerwidget.cpp \
    devicewidget.cpp \
    branchselectionwidget.cpp

HEADERS  += mainwindow.h \
    receiptmanager.h \
    joblistonreceiptdialog.h \
    connectdialog.h \
    sqlextension.h \
    setupmanager.h \
    branchwidget.h \
    changeuserdialog.h \
    usermanagementdialog.h \
    filterlineedit.h \
    fancylineedit.h \
    userloginpassmanager.h \
    userstatisticwidget.h \
    productcategorymanager.h \
    customerwidget.h \
    devicewidget.h \
    branchselectionwidget.h

FORMS    += mainwindow.ui \
    receiptmanager.ui \
    joblistonreceiptdialog.ui \
    connectdialog.ui \
    branchwidget.ui \
    changeuserdialog.ui \
    usermanagementdialog.ui \
    userloginpassmanager.ui \
    userstatisticwidget.ui \
    productcategorymanager.ui \
    devicewidget.ui \
    branchselectionwidget.ui

RESOURCES += \
    Resources.qrc

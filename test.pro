#-------------------------------------------------
#
# Project created by QtCreator 2012-10-20T11:57:57
#
#-------------------------------------------------

QT       += core gui network sql

TARGET = test
TEMPLATE = app
CONFIG(debug, debug|release):{
DEFINES+=DEBUG
DEPENDPATH += $$PWD/../qtserialport/src/serialport/debug
OBJECTS_DIR = $$PWD/build/debug/obj
} else: {
DEPENDPATH += $$PWD/../qtserialport/src/serialport/release
OBJECTS_DIR = $$PWD/build/release/obj
}
#CONFIG(debug, debug|release):DEFINES += DEBUG

greaterThan(QT_MAJOR_VERSION, 4) {
    QT       += widgets serialport
} else {
    !infile($$OUT_PWD/.qmake.cache, SERIALPORT_PROJECT_ROOT) {
	system("echo SERIALPORT_PROJECT_ROOT = $$PWD/../qtserialport >> $$OUT_PWD/.qmake.cache")
	system("echo SERIALPORT_BUILD_ROOT = $$PWD/../qtserialport >> $$OUT_PWD/.qmake.cache")
    }
    include($$SERIALPORT_PROJECT_ROOT/src/serialport/qt4support/serialport.prf)
}

message($$SERIALPORT_PROJECT_ROOT)

SOURCES += main.cpp\
        mainwindow.cpp \
    receiptmanager.cpp \
    joblistonreceiptdialog.cpp \
    connectdialog.cpp \
    sqlextension.cpp \
    setupmanager.cpp \
    branchwidget.cpp \
    closeticketwidget.cpp \
    gsmmodemmanager.cpp \
    serialportcomboboxdelegate.cpp \
    sendsms.cpp \
    modemsworkerthread.cpp

HEADERS  += mainwindow.h \
    receiptmanager.h \
    joblistonreceiptdialog.h \
    connectdialog.h \
    sqlextension.h \
    setupmanager.h \
    branchwidget.h \
    closeticketwidget.h \
    gsmmodemmanager.h \
    serialportcomboboxdelegate.h \
    sendsms.h \
    modemsworkerthread.h

FORMS    += mainwindow.ui \
    receiptmanager.ui \
    joblistonreceiptdialog.ui \
    connectdialog.ui \
    branchwidget.ui \
    closeticketwidget.ui \
    gsmmodemmanager.ui

win32-g++ {

    INCLUDEPATH += "C:/Program Files (x86)/NCReport/2.8.6.qt482.mingw.eval/include"

    CONFIG( debug, debug|release ) {
	LIBS += "C:/Program Files (x86)/NCReport/2.8.6.qt482.mingw.eval/lib/libncreportd2.a"
    } else {
	LIBS += "C:/Program Files (x86)/NCReport/2.8.6.qt482.mingw.eval/lib/libncreport2.a"
    }

}

OTHER_FILES += \
    report.xml



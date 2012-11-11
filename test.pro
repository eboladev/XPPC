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

win32-g++ {

    INCLUDEPATH += ./ncreport/include

    CONFIG( debug, debug|release ) {
	LIBS += "./ncreport/lib/libncreportd2.a"
    } else {
	LIBS += ./ncreport/lib/libncreport2.a
    }

}
#DEFINES += NCREPORT_IMPORT
#INCLUDEPATH += ./ncreport
#LIBS += ./ncreport/ncreport2.dll
#LIBS += ./ncreport/qsqlfirebird.dll
#LIBS += ./ncreport/mingwm10.dll
#LIBS += ./ncreport/libgcc_s_dw2-1.dll
#C:\Program Files (x86)\NCReport\2.8.6.qt482.mingw.eval\bin
OTHER_FILES += \
    report.xml

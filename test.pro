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

win32:CONFIG += qaxcontainer

TARGET = xppc
TEMPLATE = app

INCLUDEPATH += $$PWD

CONFIG(debug, debug|release):DEFINES += DEBUG
CONFIG(release, debug|release):DEFINES += RELEASE

win32 {
    SOURCES += $$PWD/ActiveX/acropdflib.cpp
    HEADERS += $$PWD/ActiveX/acropdflib.h
}

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
    branchselectionwidget.cpp \
    querylimitcomboboxwidget.cpp \
    ticketcommentswidget.cpp \
    guaranteeonticketreasonwidget.cpp \
    reportshandler.cpp \
    reportssettings.cpp \
    reportselectionwidget.cpp \
    employeewidget.cpp \
    employeeitemmodel.cpp \
    jobitemmodel.cpp \
    joblistitemmodel.cpp \
    permissionswidget.cpp \
    userinfowidget.cpp \
    usersandpermissionsmanager.cpp \
    contactdeveloperdialog.cpp \
    SmtpClient/src/smtpclient.cpp \
    SmtpClient/src/quotedprintable.cpp \
    SmtpClient/src/mimetext.cpp \
    SmtpClient/src/mimepart.cpp \
    SmtpClient/src/mimemultipart.cpp \
    SmtpClient/src/mimemessage.cpp \
    SmtpClient/src/mimeinlinefile.cpp \
    SmtpClient/src/mimehtml.cpp \
    SmtpClient/src/mimefile.cpp \
    SmtpClient/src/mimecontentformatter.cpp \
    SmtpClient/src/mimeattachment.cpp \
    SmtpClient/src/emailaddress.cpp \
    smtpsettingsdialog.cpp \
    applicationupdatedialog.cpp
#unix {
#    SOURCES += DocumentGenerators/writerunixautomation.cpp
#}

win32 {
    SOURCES += DocumentGenerators/writerautomation.cpp \
    DocumentGenerators/disphelper.c \
    DocumentGenerators/wordautomation.cpp

    HEADERS  += DocumentGenerators/writerautomation.h \
    DocumentGenerators/disphelper.h \
    DocumentGenerators/wordautomation.h
}

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
    branchselectionwidget.h \
    querylimitcomboboxwidget.h \
    globals.h \
    ticketcommentswidget.h \
    guaranteeonticketreasonwidget.h \
    reportshandler.h \
    reportssettings.h \
    reportselectionwidget.h \
    employeewidget.h \
    employeeitemmodel.h \
    jobitemmodel.h \
    joblistitemmodel.h \
    permissionswidget.h \
    userinfowidget.h \
    usersandpermissionsmanager.h \
    contactdeveloperdialog.h \
    SmtpClient/src/smtpclient.h \
    SmtpClient/src/quotedprintable.h \
    SmtpClient/src/mimetext.h \
    SmtpClient/src/mimepart.h \
    SmtpClient/src/mimemultipart.h \
    SmtpClient/src/mimemessage.h \
    SmtpClient/src/mimeinlinefile.h \
    SmtpClient/src/mimehtml.h \
    SmtpClient/src/mimefile.h \
    SmtpClient/src/mimecontentformatter.h \
    SmtpClient/src/mimeattachment.h \
    SmtpClient/src/emailaddress.h \
    smtpsettingsdialog.h \
    applicationupdatedialog.h
#unix {
#    HEADERS  += DocumentGenerators/writerunixautomation.h
#    }


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
    branchselectionwidget.ui \
    querylimitcomboboxwidget.ui \
    ticketcommentswidget.ui \
    guaranteeonticketreasonwidget.ui \
    reportssettings.ui \
    reportselectionwidget.ui \
    employeewidget.ui \
    permissionswidget.ui \
    userinfowidget.ui \
    contactdeveloperdialog.ui \
    smtpsettingsdialog.ui \
    applicationupdatedialog.ui

RESOURCES += \
    Resources.qrc

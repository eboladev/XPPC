#-------------------------------------------------
#
# Project created by QtCreator 2013-03-22T13:35:04
#
#-------------------------------------------------

QT       += core gui network

TARGET = GuardianApp
CONFIG   += gui

TEMPLATE = app


SOURCES += main.cpp \
    guardiancore.cpp \
    miniz.c \
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
    SmtpClient/src/emailaddress.cpp

HEADERS += \
    guardiancore.h \
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
    SmtpClient/src/emailaddress.h

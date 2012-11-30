#include <QtGui/QApplication>
#include <QSettings>
#include <QTextCodec>

#include "mainwindow.h"
#include "connectdialog.h"
#include "setupmanager.h"
#include <QSqlDatabase>
#include <QMessageBox>

#include <stdio.h>
#include <stdlib.h>

void myMessageHandler(QtMsgType type, const char *msg)
{
    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("Debug: %1").arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(msg);
        abort();
    }
    QString fileName = QDateTime::currentDateTime().toString();
    fileName.append(".txt");
    QFile outFile(fileName);
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#ifdef RELEASE
    qInstallMsgHandler(myMessageHandler);
#endif
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("MySoft");
    QCoreApplication::setOrganizationDomain("mysoft.com");
    QCoreApplication::setApplicationName("Service centre manager");
     if (!QSqlDatabase::isDriverAvailable("QFIREBIRD"))
    {
        QMessageBox::critical(0, QObject::trUtf8("Ошибка"),
                              QObject::trUtf8("Запуск программы невозможен: не найден драйвер "
                                              "QFIREBIRD"));
        return -1;
    }
/*
    if (!QSqlDatabase::isDriverAvailable("QMYSQL"))
    {
        QMessageBox::critical(0, QObject::trUtf8("Ошибка"),
                              QObject::trUtf8("Запуск программы невозможен: не найден драйвер "
                                              "QMYSQL"));
        return -1;
    }*/
    QSettings settings;

    if (settings.allKeys().isEmpty())
    {
        ConnectDialog cd;
        if (!cd.exec())
        {
            return 2;
        }
    }

    MainWindow w;
#ifdef DEBUG
    w.show();
#endif
#ifdef RELEASE
    w.showMaximized();
#endif
    return a.exec();
}

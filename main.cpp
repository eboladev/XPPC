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

void myMessageOutput(QtMsgType type, const char *msg)
{
    //in this function, you can write the message to any stream!
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s\n", msg);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s\n", msg);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s\n", msg);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s\n", msg);
        abort();
    }
}

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    qInstallMsgHandler(myMessageOutput);
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("MySoft");
    QCoreApplication::setOrganizationDomain("mysoft.com");
    QCoreApplication::setApplicationName("Service centre manager");
    qApp->addLibraryPath("./ncreport");
    if (!QSqlDatabase::isDriverAvailable("QFIREBIRD"))
    {
        QMessageBox::critical(0, QObject::trUtf8("Ошибка"),
                              QObject::trUtf8("Запуск программы невозможен: не найден драйвер "
                                              "QFIREBIRD"));
        return -1;
    }

            //    if (!QSqlDatabase::isDriverAvailable("QMYSQL"))
            //    {
            //        QMessageBox::critical(0, QObject::trUtf8("Ошибка"),
            //                              QObject::trUtf8("Запуск программы невозможен: не найден драйвер "
            //                                              "QMYSQL"));
            //        return -1;
            //    }
            qDebug() << a.libraryPaths();
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
    // w.show();
    w.showMaximized();
    return a.exec();
}

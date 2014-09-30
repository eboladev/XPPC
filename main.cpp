#include <QtGui/QApplication>
#include <QSettings>
#include <QTextCodec>
#include <QMessageBox>
#include <QDir>
#include <QDateTime>
#include <QTextStream>

#include "mainwindow.h"
#include "connectdialog.h"
#include "setupmanager.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

const QString appVersion = "0.66";

void myMessageHandler(QtMsgType type, const char *msg)
{
    QString txt;
    QString time = QDateTime::currentDateTime().toString("dd-MM-yy hh:mm:ss");
    switch (type) {
    case QtDebugMsg:
        txt = QString("%2: Debug: %1").arg(msg).arg(time);
        break;
    case QtWarningMsg:
        txt = QString("%2: Warning: %1").arg(msg).arg(time);
        break;
    case QtCriticalMsg:
        txt = QString("%2: Critical: %1").arg(msg).arg(time);
        break;
    case QtFatalMsg:
        txt = QString("%2: Fatal: %1").arg(msg).arg(time);
        abort();
    }
    QDir logs = QDir::homePath();
    if (!QDir("./logs").exists())
        logs.mkdir("./logs");
    QString filename = logs.absolutePath();
    filename.append("./logs/");
    filename.append(QDateTime::currentDateTime().toString("ddMMyyyy"));
    filename.append(".txt");
    QFile outFile(QDir::toNativeSeparators(filename));
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
    qApp->setApplicationVersion(appVersion);
    QCoreApplication::setOrganizationName("doshSoft");
    QCoreApplication::setOrganizationDomain("granet.org");
    QCoreApplication::setApplicationName("Service centre manager");

     if (!QSqlDatabase::isDriverAvailable("QPSQL"))
    {
        QMessageBox::critical(0, QObject::trUtf8("Ошибка"),
                              QObject::trUtf8("Запуск программы невозможен: не найден драйвер QPSQL"));
        return -1;
    }

    QSettings settings;

    if (settings.allKeys().isEmpty())
    {
        ConnectDialog cd;
        if (!cd.exec())
            return 2;
    }

    a.setApplicationVersion(appVersion);

    MainWindow w;
#ifdef DEBUG
    w.show();
#endif
#ifdef RELEASE
    w.showMaximized();
#endif
    return a.exec();
}

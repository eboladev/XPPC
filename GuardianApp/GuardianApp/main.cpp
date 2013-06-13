#include <QCoreApplication>
#include <QTextCodec>
#include <QDateTime>

#include "guardiancore.h"

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
    if (!QDir("./logs/GuardianApp").exists())
        logs.mkdir("./logs/GuardianApp");
    QString filename = logs.absolutePath();
    filename.append("./logs/GuardianApp/");
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
    qInstallMsgHandler(myMessageHandler);
    QCoreApplication a(argc, argv);

    GuardianCore gc(a.parent());

    return a.exec();
}

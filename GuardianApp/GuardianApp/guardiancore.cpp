#include "guardiancore.h"
#include <QSettings>
#include <QProcess>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QDateTime>
#include "miniz.c"

GuardianCore::GuardianCore(QObject *parent) :
    QObject(parent)
{
    if (checkConfig())
    {
        initProcess();
        startProgram(executable);
    }
}

GuardianCore::~GuardianCore()
{
    qDebug() << "destructor";
    delete processGlobal;
}

QString GuardianCore::getSettingsFile()
{
#ifdef Q_OS_UNIX
    return QDir::homePath() + "/guardiansettings.ini";
#else
    return QDir::homePath() + ".\\guardiansettings.ini";
#endif
}

void GuardianCore::startProgram(QString &executable_path)
{
    processGlobal->start(executable_path);
}

bool GuardianCore::checkConfig()
{
    qDebug() << "asdasd";
    QSettings s(getSettingsFile(), QSettings::IniFormat);

    //read settings from file
    //executable to guard.
    QString executable_path = s.contains("base/Executable") ? s.value("base/Executable").toString() : "";

    if (!s.contains("base/Executable"))
    {
        qDebug() << "nothing to guard, executable_path variable is empty";
        return false;
    }

    processLogs = s.value("base/CompressLogs").toBool();

    if (processLogs)
    {
        logsPath = s.contains("base/LogsDir") ? s.value("base/LogsDir").toString() : "";

        QDir logs(QDir::homePath() + logsPath);
        if (!logs.exists())
        {
            qDebug() << "LogsDir variable contains error";
            return false;
        }
        if (!logs.isReadable())
        {
            qDebug() << "LogsDir is not readable";
            return false;
        }
    }

    QFileInfo executableFile(executable_path);

    if (!executableFile.exists())
    {
        qDebug() << "executable file unreachable";
        return false;
    }

    if (!executableFile.isExecutable())
    {
        qDebug() << "cannot execute" << executable_path;
        return false;
    }
    executable = executable_path;
    qDebug() << "executable succesfully set to " << executable;
    return true;
}

void GuardianCore::initProcess()
{
    processGlobal = new QProcess(this);
    connect(processGlobal, SIGNAL(stateChanged(QProcess::ProcessState)), SLOT(onStateChanges(QProcess::ProcessState)));
}

QFileInfo GuardianCore::getLastFileInLogs()
{
    QDir logs(QDir::homePath() + logsPath);
    qDebug() << logs.exists() << logs.absolutePath();
    QFileInfoList fil = logs.entryInfoList();
    if (fil.count() == 0)
    {
        qDebug() << "no log files detected";
        return QFileInfo();
    }
    QFileInfo last = fil.at(0);
    for (int i = 0 ; i < fil.count(); ++i)
        if (last.lastModified() < fil.at(i).lastModified())
            last = fil.at(i);
    return last;
}

void GuardianCore::saveCompressedFile(QFileInfo fi)
{
    QFile source(fi.absoluteFilePath());
    if (!source.open(QIODevice::ReadOnly |QIODevice::Text))
    {
        qDebug() << "failed to open log file to read" << fi.absoluteFilePath();
        return;
    }
    QByteArray compressed = qCompress(source.readAll(),9);
    compressToZip(compressed,"test.zip",fi.fileName());
    QFile target("test2.zip");
    qDebug() << source.readAll();
    if (!target.open(QIODevice::WriteOnly))
    {
        qDebug() << "failed to open target file to save";
        return;
    }
    QFileInfo f(target);
    qDebug() << f.absoluteFilePath() << f.exists();
    target.write(source.readAll());
    target.close();
    source.close();
    qDebug() << "file saved?";
}

void GuardianCore::compressToZip(QByteArray data, QString fileName, QString archiveFileName)
{
    char *s_pComment = "";
    mz_bool status;
    qDebug() << data;
    status = mz_zip_add_mem_to_archive_file_in_place(fileName.toAscii().data(),
                                            archiveFileName.toAscii().data(),
                                            data.data(),
                                            data.length(),
                                            s_pComment,
                                            (ushort)strlen(s_pComment),
                                            MZ_BEST_COMPRESSION);
    if (!status)
    {
        qDebug() << "add mem to archive failed" << status;
        return;
    }
}

void GuardianCore::onStateChanges(QProcess::ProcessState state)
{
    QProcess *process = qobject_cast<QProcess*>(sender());
    qDebug() << process->exitCode();
    if (process->exitCode() != 0)
    {
        if (processLogs)
            saveCompressedFile(getLastFileInLogs());
        qDebug() << process->readAllStandardError() << process->readAllStandardOutput();
        initProcess();
        startProgram(executable);
    }
    if (process->exitCode() == 0 && state == 0)
        exit(0);
}


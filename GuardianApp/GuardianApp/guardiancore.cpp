#include "guardiancore.h"
#include <QSettings>
#include <QProcess>
#include <QDebug>
#include <QFileInfo>
#include <QTimer>
#include <QEventLoop>
#include <QDateTime>
#include "SmtpClient/src/mimeattachment.h"
#include "SmtpClient/src/mimehtml.h"
#include "miniz.c"

GuardianCore::GuardianCore(QObject *parent) :
    QObject(parent),
    previousValue(-1)
{    
    if (checkConfig())
    {
        initProcess();
        startProgram(executable);
    }    
}

GuardianCore::~GuardianCore()
{
    delete processGlobal;    
}

QString GuardianCore::getSettingsFile()
{
    return QDir::toNativeSeparators(QDir::homePath() + QDir::separator() + "guardiansettings.ini");
}

void GuardianCore::startProgram(QString &executable_path)
{
    #ifdef Q_OS_WIN32
    QSettings s("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\Windows Error Reporting", QSettings::NativeFormat);
    if (previousValue == -1)
        previousValue = s.value("DontShowUI").toInt();
    if (s.value("DontShowUI").toInt() == 0)
        s.setValue("DontShowUI",1);
    #endif
    processGlobal->start(executable_path);
}

bool GuardianCore::checkConfig()
{
    QSettings s(getSettingsFile(), QSettings::IniFormat);
    qDebug() << getSettingsFile();
    QString executable_path = s.contains("base/Executable") ? s.value("base/Executable").toString() : "";

    if (!s.contains("base/Executable"))
    {
        fillExampleConfig();
        qDebug() << "nothing to guard, executable_path variable is empty";
        return false;
    }

    processLogs = s.contains("base/ProcessLogs") ?  s.value("base/ProcessLogs").toBool() : false ;

    if (processLogs)
    {
        compressLogs = s.contains("base/CompressLogs") ? s.value("base/CompressLogs").toBool() : false;

        sendLog = s.contains("email/SendLog") ? s.value("email/SendLog").toBool() : false;

        if (sendLog)
        {
            smtpAddress = s.contains("email/SMTPAddress") ? s.value("email/SMTPAddress").toString() : "";
            smtpPort = s.contains("email/SMTPPort") ? s.value("email/SMTPPort").toInt() : -1;
            smtpUserName = s.contains("email/SMTPLogin") ? s.value("email/SMTPLogin").toString() : "";
            smtpPassword = s.contains("email/SMTPPass") ? s.value("email/SMTPPass").toString() : "";
            QByteArray ba2 = s.value("email/SMTPPass").toByteArray();
            encryptDecrypt(ba2);
            smtpPassword = QString::fromUtf8(ba2.data(), ba2.count());
            qDebug() << smtpPassword << Q_FUNC_INFO;
            senderName = s.contains("email/SMTPSenderName") ? s.value("email/SMTPSenderName").toString() : "";
            senderEMail = s.contains("email/SMTPSenderAddress") ? s.value("email/SMTPSenderAddress").toString() : "";
            mailTo = s.contains("email/MailTo") ? s.value("email/MailTo").toString() : "";
            if (smtpAddress.isEmpty() ||
                    smtpPort == -1 ||
                    smtpUserName.isEmpty() ||
                    smtpPassword.isEmpty() ||
                    senderName.isEmpty() ||
                    senderEMail.isEmpty() ||
                    mailTo.isEmpty())
            {
                fillExampleConfig();
                sendLog = false;
            }
        }

        QString logsPath = s.contains("base/LogsDir") ? s.value("base/LogsDir").toString() : "";

        logsDir = QDir(QDir::toNativeSeparators(logsPath));
        qDebug() << "logs dir" << logsDir.absolutePath() << logsDir.path();

        QString logsSavePath = s.contains("base/SaveLogsDir") ? s.value("base/SaveLogsDir").toString() : "";
        logsSaveDir = QDir(QDir::toNativeSeparators(logsSavePath));

        qDebug() << "logs save dir" << logsSaveDir.absolutePath() << logsSaveDir.path();
    }

    QFileInfo executableFile(executable_path);

    if (!executableFile.exists())
    {
        fillExampleConfig();
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
    QFileInfoList fil = logsDir.entryInfoList();    
    if (fil.count() < 2)
    {
        qDebug() << "no log files detected";
        return QFileInfo();
    }    
    QFileInfo last = fil.at(2);
    for (int i = 0 ; i < fil.count(); ++i)
    {
       //qDebug() << fil.at(i).absoluteFilePath() << fil.at(i).isDir() << Q_FUNC_INFO;
        if (!fil.at(i).isDir())
        if (last.lastModified() < fil.at(i).lastModified())
            last = fil.at(i);
    }
   // qDebug() << last.absoluteFilePath() << last.isDir() << Q_FUNC_INFO;
    return last;
}

void GuardianCore::saveLogFile(QFileInfo fi)
{
    qDebug() << fi.absolutePath() << fi.fileName() << Q_FUNC_INFO;
    QTimer t;
    QEventLoop el;
    connect(&t, SIGNAL(timeout()), &el, SLOT(quit()));
    t.start(6000);
    el.exec();
    QFile source(fi.absoluteFilePath());
    if (!source.open(QIODevice::ReadOnly |QIODevice::Text))
    {
        qDebug() << "failed to open log file to read" << fi.absoluteFilePath();
        return;
    }
    QString fileName = QString("crash_%0").arg(QDateTime::currentDateTime().toString("dd-MM-yyyy_hh-mm-ss"));
    QString filePath = logsSaveDir.absolutePath().append(QDir::separator());
    filePath.append(fileName);
    if (compressLogs)
    {
        filePath.append(".zip");
        qDebug() << "file " << fileName << QDir::toNativeSeparators(filePath) << "compressed" << compressToZip(source.readAll(), QDir::toNativeSeparators(filePath), fi.fileName());
        if (sendLog)
        {
            fileName.append(".zip");
            QFile archive(fileName);
            MimeAttachment *mime = new MimeAttachment(new QFile(fileName));
            archive.open(QIODevice::ReadOnly | QIODevice::Text);
            mime->setContent(archive.readAll());
            archive.close();            
            currentEMail.attachementsList.append(mime);
            QFileInfo filein = QFileInfo(QDir(""),"stackdump.txt");
            qDebug() << filein.filePath() << filein.absoluteFilePath() << filein.exists() << Q_FUNC_INFO;
            if (filein.exists())
            {
                MimeAttachment *mimeStackDump = new MimeAttachment(new QFile("stackdump_.txt"));
                QFile stackDump(filein.fileName());
                stackDump.open(QIODevice::ReadOnly | QIODevice::Text);
                QByteArray ba = stackDump.readAll();
                //qDebug() << "are ya fucking here?" << ba.length() << ba.data() << stackDump.fileName() << stackDump.exists() << Q_FUNC_INFO;
                mimeStackDump->setContent(ba);
                stackDump.close();
                currentEMail.attachementsList.append(mimeStackDump);
            }

            QString mailSubject;
            mailSubject =  QString("Application %0 has crashed").arg(executable);
            currentEMail.mailSubject.append(mailSubject);
            currentEMail.mailBody.append("GuardianApp has sent you attachment with crash log");
            currentEMail.recipientsList.append(new EmailAddress(mailTo));
            sendMail();
        }
    }
    else
    {
        filePath.append(".txt");
        source.copy(filePath);
    }
    source.close();
}

bool GuardianCore::compressToZip(const QByteArray& data,const QString& fileName, const QString& archiveFileName)
{    
    char *s_pComment = "";
    mz_bool status;

    status = mz_zip_add_mem_to_archive_file_in_place(fileName.toAscii(),
                                            archiveFileName.toAscii(),
                                            data.data(),
                                            data.length(),
                                            s_pComment,
                                            (ushort)strlen(s_pComment),
                                            MZ_BEST_COMPRESSION);
    return status;
}

void GuardianCore::sendMail()
{
    // создаём объект smtp клиента
    SmtpClient smtp(smtpAddress, smtpPort, SmtpClient::TcpConnection);

    // имя пользователя и пароль
    smtp.setUser(smtpUserName);
    smtp.setPassword(smtpPassword);

    // создаём сообщение
    MimeMessage message;
    EmailAddress ema(senderEMail, senderName);

    message.setSender(&ema);
    message.setSubject(currentEMail.mailSubject);

    // добавляем получателей
    foreach (EmailAddress *ea, currentEMail.recipientsList)
        message.addRecipient(ea);

    // текст нашего письма
    MimeHtml text;
    text.setHtml(currentEMail.mailBody);
    message.addPart(&text);

    // добавляем вложения
    foreach(MimePart *mime, currentEMail.attachementsList)
    {
        message.addPart(mime);
        qDebug() << "mime^" << mime->getContentName() << mime->getContent().size();
    }

    // теперь мы можем послать нашу почту

    smtp.setEmailId(currentEMail.id);

    if (smtp.connectToHost())
    {
        if (smtp.login())
        {
            if (smtp.sendMail(message))
                qDebug() << "email has been sent";            
        }
        else
            qDebug() << Q_FUNC_INFO << "failed to login";

        smtp.quit();
    }
    else
        qDebug() << Q_FUNC_INFO << "failed to connect to host";

    foreach (EmailAddress *ea, currentEMail.recipientsList)
        message.deleteRecipient(ea);
    foreach(MimePart *mime, currentEMail.attachementsList)
        message.deletePart(mime);
}

void GuardianCore::fillExampleConfig()
{
    QSettings s(getSettingsFile(),QSettings::IniFormat);
    QStringList sl;
    sl << "base/Executable" << "base/ProcessLogs" << "base/LogsDir"
       << "base/CompressLogs" << "base/SaveLogsDir" <<
          "email/SendLog" << "email/MailTo" << "email/SMTPAddress" <<
          "email/SMTPPort" << "email/SMTPLogin" << "email/SMTPPass" <<
          "email/SMTPSenderName" << "email/SMTPSenderAddress";
    foreach (QString value, sl)
        if (!s.contains(value))
            s.setValue(value,QVariant());
    QTimer t;
    QEventLoop el;
    connect(&t, SIGNAL(timeout()), &el, SLOT(quit()));
    t.start(3000);
    el.exec();
    exit(-1);
}

void GuardianCore::onStateChanges(QProcess::ProcessState state)
{
    QProcess *process = qobject_cast<QProcess*>(sender());

    if (process->exitCode() != 0)
    {
        if (processLogs)        
            saveLogFile(getLastFileInLogs());
        initProcess();
        startProgram(executable);
    }    
    if (process->exitCode() == 0 && state == 0)
    {
#ifdef Q_OS_WIN32
        QSettings s("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\Windows Error Reporting", QSettings::NativeFormat);
        if (s.value("DontShowUI").toInt() != previousValue)
            s.setValue("DontShowUI",previousValue);
#endif
        exit(0);
    }
}

void GuardianCore::encryptDecrypt(QByteArray &ba)
{
    char c = 55;
    for (int i=0; i<ba.count();++i){
        ba[i] = ba.at(i) ^ c;
        c += 34;
    }
}

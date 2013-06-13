#ifndef GUARDIANCORE_H
#define GUARDIANCORE_H

#include <QObject>
#include <QProcess>
#include <QFileInfo>
#include <QDir>
#include <QQueue>
#include "SmtpClient/src/smtpclient.h"

struct EMailToSend
{
    // Тело письма
    QString mailBody;

    // тема письма
    QString mailSubject;

    // список вложений
    QList<MimePart*> attachementsList;

    // список получателей
    QList<EmailAddress*> recipientsList;

    // идентификатор
    int id;
};

class GuardianCore : public QObject
{
    Q_OBJECT
public:
    explicit GuardianCore(QObject *parent = 0);
    ~GuardianCore();

private:
    QString getSettingsFile();
    void startProgram(QString& executable_path);
    bool checkConfig();
    void initProcess();
    QFileInfo getLastFileInLogs();
    void saveLogFile(QFileInfo);
    bool compressToZip(const QByteArray& data,const QString& fileName, const QString& archiveFileName);
    void sendMail();
    void fillExampleConfig();

private Q_SLOTS:
    void onStateChanges(QProcess::ProcessState);

private:
    void encryptDecrypt(QByteArray &ba);

private:
    QString executable;
    bool processLogs;
    bool compressLogs;
    bool sendLog;
    int previousValue;
    QDir logsDir;
    QDir logsSaveDir;
    QProcess* processGlobal;
    QString smtpAddress;
    int smtpPort;
    QString smtpUserName;
    QString smtpPassword;
    QString senderName;
    QString senderEMail;
    QString mailTo;
    EMailToSend currentEMail;
};

#endif // GUARDIANCORE_H

#ifndef GUARDIANCORE_H
#define GUARDIANCORE_H

#include <QObject>
#include <QProcess>
#include <QFileInfo>

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
    void saveCompressedFile(QFileInfo);
    void compressToZip(QByteArray data, QString fileName, QString archiveFileName);

private slots:
    void onStateChanges(QProcess::ProcessState);

private:
    QString executable;
    bool processLogs;
    QString logsPath;
    QProcess* processGlobal;
};

#endif // GUARDIANCORE_H

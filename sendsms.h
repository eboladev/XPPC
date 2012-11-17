#ifndef SENDSMS_H
#define SENDSMS_H

#include <QObject>
#include <QThread>

class ModemsWorkerThread;
class SendSMS : public QObject
{
    Q_OBJECT
public:
    explicit SendSMS(QObject *parent = 0);
    ~SendSMS();
    // добавить модем
    bool addModem(const QString &modemName, const QString &portName, const QString &telNumber = trUtf8(""));
    // удалить модем
    bool removeModem(const QString &modemName);
    // закрыть порт, на котором "сидит" модем
    bool closeModem(const QString &modemName);
    // открыть порт, на которм "сидит" модем, и начть его мониторинг
    bool openModem(const QString &modemName);
    // проверка на открытость порта
    bool portIsOpen(const QString &modemName);
    // проверка на наличиче порта
    bool portIsPresent(const QString &modemName);
    // проверка на регистрацию в сети модема
    bool isRegisteredInNetwork(const QString &modemName);
    // поменять имя модема
    bool changeModemName(const QString &oldModemName, const QString &newModemName);
    // установить кол-во микросекунд перед "отбоем" при входящем звонке
    void setMsecToHangDown(const int &msec);


//    QString portName(const QString &modemName);
//    int signalLevel(const QString &modemName);

public Q_SLOTS:
    // послать СМС
    bool sendSMS(const QString &smsText, const QString &phoneNumber, const int &idSMS);
    // удалить все модемы
    void removeAllModems();

Q_SIGNALS:
    void smsDelivered(QString modemName, int idSMS, bool delivered);
    void smsSended(QString modemName, int idSMS);
    void startSendingSMS(QString modemName, int idSMS);
    void errorWhileSendingSMS(QString modemName, int idSMS);
    void incomingRing(QString modemName, QString telNumber);
    void incomingSMS(QString modemName, QString phoneNumber, QString smsTest);
    void signalLevels(QString modemName, int rssi, int bor);
    void portStateChanged(QString modemName, bool isOpen);

    void isModemResponse(QString modemName, bool response);
    void portIsLost(QString modemName);
    void portIsRestored(QString modemName);
    void CMEError(QString modemName, int errorNum);
    void CMSError(QString modemName, int errorNum);
    void error(QString modemName, QString lastCommand);
    void isRegisteredInNetwork (QString modemName, int status); //old name OperatorStatus
    void operatorName(QString modemName, QString operatorN);

private:
    QThread m_checkPortPresentThread;
    QThread m_modemsThread;
    ModemsWorkerThread *m_modemsWorker;


    
};

#endif // SENDSMS_H

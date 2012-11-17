#ifndef MODEMSWORKERTHREAD_H
#define MODEMSWORKERTHREAD_H

#include <QObject>
#include <QHash>
#include <QtAddOnSerialPort/serialport.h>
#include <QtAddOnSerialPort/serialportinfo.h>
#include <QStringList>
static const int commandsTimeout = 5000;
static const int sendSmsTimeout = 15000;
static const int waitAfterTimeoutBeforeReinitialize = 20000;

QT_USE_NAMESPACE_SERIALPORT
class QTimer;
class QThread;

enum modemStatus {
    waitData,
    waitSmsResponse1,
    waitSmsResponse2,
    waitSimpleResponse,
    waitInitializeCommandResponse,
    waitPeriodForReinitialize
};

struct modemInfo {
    QString modemName;
    QString portName;
    QString telephoneNumber;
    QString networkName;
    QString currentSms;
    QString currentPduForSms;
    QString phoneOfRecipient;
    QString lastGSMCommand;
    QString lastIncomingRingNumber;
    QString lastIncomingSmsNumber;
    QStringList initializeCommands;
    QStringList simpleCommands;
    QStringList complexCommands;
    QByteArray *buffer;
    SerialPort *serialPort;
    QTimer *timer;
    modemStatus status;
    int currentBufferPosition;
    int ringNumber;
    int internalCounter;
    int m_rssi;
    int m_bor;
    bool waitingSMSPdu;
    bool waitingSMSDeliveryPDU;
    bool isResponse;
    bool isOpen;
    bool isBusy;
    bool isDisapear;
    bool isInitialized;
    bool isRegisteredInNetwork;
    bool inIncomingRingMode;
    int idSMS;
    QHash<int, int> smsPendingTheReceipt;
};

struct sms {
    QString smsTest;
    QString telephoneNumber;
    bool isSendingNow;
    int smsId;
};

class ModemsWorkerThread : public QObject
{
    Q_OBJECT
public:
    explicit ModemsWorkerThread(QObject *parent = 0);
    ~ModemsWorkerThread();
public Q_SLOTS:


    bool addModem(const QString &modemName, const QString &portName, const QString &telNumber = trUtf8(""));
    bool removeModem(const QString &modemName);
    bool closeModem(const QString &modemName);
    bool openModem(const QString &modemName);
    bool portIsOpen(const QString &modemName);
    bool portIsPresent(const QString &modemName);
    bool isRegisteredInNetwork(const QString &modemName);

    /*
    QString portName(const QString &modemName);
    int signalLevel(const QString &modemName);
    */
    bool sendSMS(const QString &smsText, const QString &phoneNumber, const int &idSMS);
    bool changeModemName(const QString &oldModemName, const QString &newModemName);
    void removeAllModems();
    void setMsecToHangDown(const int &msec) {msecToHangDown = msec;}

private:
    void addInitializeCommand(QStringList &stringList);
    QString getPDUText(QString telNum, QString smsText);
    void close(modemInfo *pModemInfo);
    bool open(modemInfo *pModemInfo);
    QString getASCIIFrom7BitCodedString(const QString &str);
    bool regExpHelper(const QString &regExp, const QString &str, int &error);
    bool containsCMEError(const QString &textLine, int &error);
    bool containsCMSError(const QString &textLine, int &error);
    bool parseOkResponse(const QString &textLine, modemInfo * const pModemInfo);
    bool parseErrorResponse(const QString &textLine, modemInfo * const pModemInfo);
    bool parseIncomingRing(const QString &textLine, modemInfo * const pModemInfo);
    bool parseIncomingSMS(const QString &textLine, modemInfo * const pModemInfo);
    bool parseOperatorName(const QString &textLine, modemInfo * const pModemInfo);
    bool parseSignalLevel(const QString &textLine, modemInfo * const pModemInfo);
    bool parseRegisterInNetwork(const QString &textLine, modemInfo * const pModemInfo);
    bool parseDeliveryReport(const QString &textLine, modemInfo * const pModemInfo);
    bool parseCommonAnswers(const QString &textLine, modemInfo * const pModemInfo);
    bool parseSimpleAnswert(const QString &textLine, modemInfo * const pModemInfo);
    bool parsePDU(const QString &smsPDU, QString &_telephone, QString &_smsText);

private Q_SLOTS:
    void onReadyRead();
    void onCheckPortsTimer();
    void onTimeoutWatcherTimer();
    void onCheckOutputBuffers();
    void onHangDownTimer();

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
    // список модемов
    QHash <QString, modemInfo> modemsList;
    // список СМС, в очереди на отправку
    QList<sms> smsList;
    // поток для таймера проверки порта
    QThread *checkPortThread;
    // сам таймер проверки порта
    QTimer *checkPortTimer;
    // таймер проверки выходных буферов
    QTimer *checkOutputBuffersTimer;
    // количество милисекунд до отбоя входящего звонка
    int msecToHangDown;
};

#endif // MODEMSWORKERTHREAD_H

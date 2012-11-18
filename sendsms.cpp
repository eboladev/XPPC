#include "sendsms.h"
#include "modemsworkerthread.h"


SendSMS::SendSMS(QObject *parent) :
    QObject(parent)
{
    m_modemsThread.start();
    m_modemsWorker = new ModemsWorkerThread();
    m_modemsWorker->moveToThread(&m_modemsThread);

    // "сквозные" коннекты
    connect(m_modemsWorker, SIGNAL(CMEError(QString,int)), this, SIGNAL(CMEError(QString,int)));
    connect(m_modemsWorker, SIGNAL(CMSError(QString,int)), this, SIGNAL(CMSError(QString,int)));
    connect(m_modemsWorker, SIGNAL(error(QString,QString)), this, SIGNAL(error(QString,QString)));
    connect(m_modemsWorker, SIGNAL(errorWhileSendingSMS(QString,int)), this, SIGNAL(errorWhileSendingSMS(QString,int)));
    connect(m_modemsWorker, SIGNAL(incomingRing(QString,QString)), this, SIGNAL(incomingRing(QString,QString)));
    connect(m_modemsWorker, SIGNAL(incomingSMS(QString,QString,QString)), this, SIGNAL(incomingSMS(QString,QString,QString)));
    connect(m_modemsWorker, SIGNAL(isModemResponse(QString,bool)), this, SIGNAL(isModemResponse(QString,bool)));
    connect(m_modemsWorker, SIGNAL(isRegisteredInNetwork(QString,int)), this, SIGNAL(isRegisteredInNetwork(QString,int)));
    connect(m_modemsWorker, SIGNAL(operatorName(QString,QString)), this, SIGNAL(operatorName(QString,QString)));
    connect(m_modemsWorker, SIGNAL(portIsLost(QString)), this, SIGNAL(portIsLost(QString)));
    connect(m_modemsWorker, SIGNAL(portIsRestored(QString)), this, SIGNAL(portIsRestored(QString)));
    connect(m_modemsWorker, SIGNAL(portStateChanged(QString,bool)), this, SIGNAL(portStateChanged(QString,bool)));
    connect(m_modemsWorker, SIGNAL(signalLevels(QString,int,int)), this, SIGNAL(signalLevels(QString,int,int)));
    connect(m_modemsWorker, SIGNAL(smsDelivered(QString,int,bool)), this, SIGNAL(smsDelivered(QString,int,bool)));
    connect(m_modemsWorker, SIGNAL(smsSended(QString,int)), this, SIGNAL(smsSended(QString,int)));
    connect(m_modemsWorker, SIGNAL(startSendingSMS(QString,int)), this, SIGNAL(startSendingSMS(QString,int)));

}

SendSMS::~SendSMS()
{
    QMetaObject::invokeMethod(m_modemsWorker, "removeAllModems", Qt::BlockingQueuedConnection);
    QMetaObject::invokeMethod(m_modemsWorker, "deleteLater", Qt::BlockingQueuedConnection);
    m_modemsThread.quit();
    if (!m_modemsThread.wait(10000))
        m_modemsThread.terminate();
}

Q_GLOBAL_STATIC(SendSMS, singlton_instance)
SendSMS *SendSMS::instance()
{
    return singlton_instance();
}

QStringList SendSMS::getAvailablePorts()
{
    QList<QtAddOn::SerialPort::SerialPortInfo> si = QtAddOn::SerialPort::SerialPortInfo::availablePorts();
    QStringList portNames;
    foreach(QtAddOn::SerialPort::SerialPortInfo s, si)
        portNames << s.portName();
    return portNames;
}

bool SendSMS::addModem(const QString &modemName, const QString &portName, const QString &telNumber)
{
    bool retVal;
    QMetaObject::invokeMethod(m_modemsWorker, "addModem",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(bool, retVal),
                              Q_ARG(QString, modemName), Q_ARG(QString, portName), Q_ARG(QString, telNumber));
    return retVal;
}


bool SendSMS::sendSMS(const QString &smsText, const QString &phoneNumber, const int &idSMS)
{
    bool retVal;
    QMetaObject::invokeMethod(m_modemsWorker, "sendSMS",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(bool, retVal),
                              Q_ARG(QString, smsText), Q_ARG(QString, phoneNumber), Q_ARG(int, idSMS));
    return retVal;
}

void SendSMS::removeAllModems()
{
    QMetaObject::invokeMethod(m_modemsWorker, "removeAllModems", Qt::QueuedConnection);
}

bool SendSMS::changeModemName(const QString &oldModemName, const QString &newModemName)
{
    bool retVal;
    QMetaObject::invokeMethod(m_modemsWorker, "changeModemName",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(bool, retVal),
                              Q_ARG(QString, oldModemName), Q_ARG(QString, newModemName));
    return retVal;
}

void SendSMS::setMsecToHangDown(const int &msec)
{
    m_modemsWorker->setMsecToHangDown(msec);
}



bool SendSMS::removeModem(const QString &modemName)
{
    bool retVal;
    QMetaObject::invokeMethod(m_modemsWorker, "removeModem",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(bool, retVal),
                              Q_ARG(QString, modemName));
    return retVal;
}

bool SendSMS::closeModem(const QString &modemName)
{
    bool retVal;
    QMetaObject::invokeMethod(m_modemsWorker, "closeModem",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(bool, retVal),
                              Q_ARG(QString, modemName));
    return retVal;
}

bool SendSMS::openModem(const QString &modemName)
{
    bool retVal;
    QMetaObject::invokeMethod(m_modemsWorker, "openModem",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(bool, retVal),
                              Q_ARG(QString, modemName));
    return retVal;
}

bool SendSMS::portIsOpen(const QString &modemName)
{
    bool retVal;
    QMetaObject::invokeMethod(m_modemsWorker, "portIsOpen",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(bool, retVal),
                              Q_ARG(QString, modemName));
    return retVal;
}

bool SendSMS::portIsPresent(const QString &modemName)
{
    bool retVal;
    QMetaObject::invokeMethod(m_modemsWorker, "portIsPresent",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(bool, retVal),
                              Q_ARG(QString, modemName));
    return retVal;
}

bool SendSMS::isRegisteredInNetwork(const QString &modemName)
{
    bool retVal;
    QMetaObject::invokeMethod(m_modemsWorker, "portIsPresent",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(bool, retVal),
                              Q_ARG(QString, modemName));
    return retVal;
}




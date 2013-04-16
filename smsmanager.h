#ifndef SMSMANAGER_H
#define SMSMANAGER_H

#include <QObject>
#include <QHash>
#include <QStringList>
#include <QDateTime>

#include "globals.h"

#define smsManager (static_cast<SmsManager*>(SmsManager::instance()))

class QTimer;
class MainSMSHandler;

const int WAITTIME = 3000;

struct SmsStatus
{
    QString text;
    QStringList recipients;
    int ticketId;
    QDateTime lastStatusTime;
    int id;
    int gwId;
    XPPC::SmsStatus status = XPPC::NotFound;
    QTimer* timer = 0;
    int trys = 3;
};

class SmsManager : public QObject
{
    Q_OBJECT
public:
    explicit SmsManager(QObject *parent = 0);
    static SmsManager* instance();

    void initGsmGateway();
    void loadGsmGatewaySettings();

    void sendSms(const QString& textSms, const QStringList& recipients, const int& ticket_id = -1);
    SmsStatus getSmsStatus(const int& ticket_id);
    void requestBalance();

private slots:
    void onSendedIds(const QHash<QString,QString> idToRecepientHash, const QString& sendId);
    void onSmsStatus(const QHash<QString,QString> idToStatusHash);
    void onGwBalance(const double& currency);
    void onSmsPrice(const int& count, const int& parts, const double& price, const double& currency);
    void onError(const int& errorCode, const QString& error);

    void onTimeout();

Q_SIGNALS:
    void smsStatus(const QString& idSMS, const XPPC::SmsStatus& status);    
    void gwBalance(const double& currency);
    /*void smsPrice(const QString& gwId, const int& count, const int& parts, const int& price, const double& currency);
    void error(const QString& gwId, const int& errorCode, const QString& error);*/
    void smsSended(const int& ticket_id);
    void smsDelivered(const int& ticket_id);
    void smsNonDelivered(const int& ticket_id);
    void smsInProcess(const int& id);

private:
    void updateSmsStatus(const XPPC::SmsStatus& status, const int& id);

private:
    MainSMSHandler* mainSms;
    QList<SmsStatus> smsList;
};

#endif // SMSMANAGER_H

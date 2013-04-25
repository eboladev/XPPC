#include "smsmanager.h"

#include <QDebug>
#include <QSettings>
#include <QTimer>

#include "mainsmshandler.h"
#include "setupmanager.h"

SmsManager::SmsManager(QObject *parent) :
    QObject(parent)
{
    initGsmGateway();
}

Q_GLOBAL_STATIC(SmsManager, singlton_instance)
SmsManager *SmsManager::instance()
{
    return singlton_instance();
}

void SmsManager::initGsmGateway()
{
    mainSms = new MainSMSHandler(this);
    loadGsmGatewaySettings();
    connect(mainSms,    SIGNAL(sendedIds(QHash<QString,QString>,QString)),
            this,       SLOT(onSendedIds(QHash<QString,QString>,QString)));
    connect(mainSms,    SIGNAL(smsStatus(QHash<QString,QString>)),
            this,       SLOT(onSmsStatus(QHash<QString,QString>)));
    connect(mainSms,    SIGNAL(balanceCash(double)),
            this,       SLOT(onGwBalance(double)));
    connect(mainSms,    SIGNAL(smsPrice(int,int,double,double)),
            this,       SLOT(onSmsPrice(int,int,double,double)));
    connect(mainSms,    SIGNAL(error(int,QString)),
            this,       SLOT(onError(int,QString)));
    mainSms->balance();
}

void SmsManager::loadGsmGatewaySettings()
{
    QSettings s;
    QString gw = QString("smsgateway/%0/").arg(XPPC::MainSms);
    mainSms->setLogin(s.value(gw.append("login")).toString());
    mainSms->setPassword(s.value(gw.append("password")).toString());
    mainSms->setSenderName(s.value(gw.append("sendername")).toString());
    mainSms->setTestMode(true);
}

void SmsManager::sendSms(const QString &textSms, const QStringList &recipients, const int &ticket_id)
{
    qDebug() << recipients << Q_FUNC_INFO;
    mainSms->send(textSms,recipients,QString::number(ticket_id));
    QSqlQuery q;
    if (!setupManager->getSqlQueryForDB(q))
        return;
    q.prepare("insert into sended_sms(smstext,recipients,status,ticket_id) VALUES(?,?,?,?) returning id");
    q.addBindValue(textSms);
    q.addBindValue(recipients.join(";"));
    q.addBindValue(XPPC::InProcess);
    q.addBindValue(ticket_id);
    q.exec();
    q.next();
    SmsStatus sms;
    sms.text = textSms;
    sms.id = q.value(0).toInt();
    sms.recipients = recipients;
    sms.ticketId = ticket_id;
    sms.status = XPPC::InProcess;
    sms.lastStatusTime = QDateTime::currentDateTime();
    smsList.append(sms);
    mainSms->balance();
}

SmsStatus SmsManager::getSmsStatus(const int &ticket_id)
{
    SmsStatus ss;

    QSqlQuery q;

    if (!setupManager->getSqlQueryForDB(q))
        return ss;

    q.prepare("select status,last_status_time from sended_sms where ticket_id = ?");
    q.addBindValue(ticket_id);
    q.exec();

    if (q.next())
    {
        ss.status = static_cast<XPPC::SmsStatus>(q.value(0).toInt());
        ss.lastStatusTime = q.value(1).toDateTime();
    }

    return ss;
}

void SmsManager::requestBalance()
{
    mainSms->balance();
}

void SmsManager::onSendedIds(const QHash<QString, QString> idToRecepientHash, const QString &sendId)
{
    qDebug() << idToRecepientHash << sendId << Q_FUNC_INFO;
    QHashIterator<QString,QString> it(idToRecepientHash);
    while (it.hasNext())
    {
        it.next();        
        for (int i = 0 ; i < smsList.count() ; ++i)
        {
            if (smsList.at(i).ticketId == sendId.toInt())
            {
                SmsStatus ss = smsList.at(i);
                ss.status = XPPC::Sended;
                ss.gwId = it.value().toInt();
                smsList.replace(i,ss);
                qDebug() << ss.status << ss.lastStatusTime << ss.text << ss.recipients << Q_FUNC_INFO;
                updateSmsStatus(ss.status,ss.id);
                emit smsSended(ss.id);
                break;
            }
        }
    }
    mainSms->status(idToRecepientHash.values());
}

void SmsManager::onSmsStatus(const QHash<QString, QString> idToStatusHash)
{
    //QHash(("0", "not found")("1", "not found")) void SmsManager::onSmsStatus(QHash<QString, QString>)
    /*
enqueued 	В очереди на отправление
accepted 	Принято к отправлению
delivered 	Доставлено
non-delivered 	Отклонено
     */
    qDebug() << idToStatusHash << Q_FUNC_INFO;
    QHashIterator<QString,QString> it(idToStatusHash);
    while (it.hasNext())
    {
        it.next();        
        for (int i = 0 ; i < smsList.count() ; ++i)
        {
            //qDebug() << smsList.at(i).gwId  << smsList.count() << Q_FUNC_INFO;
            if (smsList.at(i).gwId == it.key().toInt())
            {
                SmsStatus ss = smsList.at(i);
                if ((it.value() == QString("enqueued")) ||
                (it.value() == QString("accepted")))
                {
                    ss.status = XPPC::InQueue;
                    updateSmsStatus(ss.status,ss.id);
                    emit smsInProcess(ss.id);
                    if (!ss.timer)
                    {
                        ss.timer = new QTimer(this);
                        connect(ss.timer, SIGNAL(timeout()),
                                this, SLOT(onTimeout()));
                        qDebug() << "created a timer object";
                        ss.timer->start(WAITTIME);
                    }
                    qDebug() << ss.trys << Q_FUNC_INFO;
                    ss.timer->setProperty("trys",--ss.trys);
                    ss.timer->setProperty("gwid",ss.gwId);
                    smsList.replace(i,ss);
                }
                if (it.value() == QString("delivered"))
                {
                    updateSmsStatus(XPPC::Delivered,ss.id);
                    if (ss.timer)
                    {
                        ss.timer->stop();
                        ss.timer->deleteLater();
                    }
                    emit smsDelivered(ss.id);
                    smsList.removeAt(i);
                }
                if ((it.value() == QString("non-delivered")) ||
                        (it.value() == QString("not found")))
                {
                    updateSmsStatus(XPPC::NotFound,ss.id);
                    if (ss.timer)
                    {
                        ss.timer->stop();
                        ss.timer->deleteLater();
                    }
                    emit smsNonDelivered(ss.id);
                    smsList.removeAt(i);
                }
            }
        }
    }
}

void SmsManager::onGwBalance(const double &currency)
{
    emit gwBalance(currency);
    qDebug() << currency << Q_FUNC_INFO;
}

void SmsManager::onSmsPrice(const int &count, const int &parts, const double &price, const double &currency)
{
    qDebug() << count << parts << price << currency << Q_FUNC_INFO;
}

void SmsManager::onError(const int &errorCode, const QString &error)
{
    qDebug() << errorCode << error << Q_FUNC_INFO;
}

void SmsManager::onTimeout()
{
    int gwid = sender()->property("gwid").toInt();
    qDebug() << Q_FUNC_INFO << gwid;
    if (sender()->property("trys").toInt() > 0)
        mainSms->status(QStringList() << QString::number(gwid));
    else
    {
        for (int i = 0 ; i < smsList.count(); ++i)
            if (smsList.at(i).gwId == gwid)
            {
                updateSmsStatus(XPPC::NonDelivered,smsList.at(i).id);
                emit smsNonDelivered(smsList.at(i).id);
                smsList.at(i).timer->stop();
                smsList.at(i).timer->deleteLater();
                smsList.removeAt(i);
            }
    }
}

void SmsManager::updateSmsStatus(const XPPC::SmsStatus &status, const int &id)
{
    qDebug() << status << id << Q_FUNC_INFO;
    QSqlQuery q;
    if (!setupManager->getSqlQueryForDB(q))
        return;
    q.prepare("update sended_sms set status = ? where id = ?");
    q.addBindValue(status);
    q.addBindValue(id);
    if (!q.exec())
        qDebug() << q.lastError() << q.lastQuery() << Q_FUNC_INFO;
}

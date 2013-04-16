#ifndef ABSTRACTSMSGATEWAY_H
#define ABSTRACTSMSGATEWAY_H

#include <QObject>
#include <QHash>

class AbstractSMSGateway : public QObject
{
    Q_OBJECT

public:
    explicit AbstractSMSGateway(QObject* parent);
    virtual void send(const QString& smsText,
              const QStringList& recipientsList, const QString& sendId) = 0;
    virtual void status(const QStringList& messageIds) = 0;
    virtual void balance() = 0;

    QString getLogin() const;
    void setLogin(const QString &value);

    QString getPassword() const;
    void setPassword(const QString &value);

    QString getSenderName() const;
    void setSenderName(const QString &value);

    bool getEnabled() const;
    void setEnabled(bool value);

    QString getClassId() const;
    void setClassId(const QString &value);

signals:
    void sendedIds(const QHash<QString,QString> idToRecepientHash, const QString& sendId);
    void smsStatus(const QHash<QString,QString> idToStatusHash);
    void balanceCash(const double& currency);
    void smsPrice(const int& count, const int& parts, const double& price, const double& currency);
    void error(const int& errorCode, const QString& error);

private:
    QString login;
    QString password;
    QString senderName;
    bool enabled;
    QString classId;
};
#endif // ABSTRACTSMSGATEWAY_H

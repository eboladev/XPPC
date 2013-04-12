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
              const QStringList& recipientsList) = 0;
    virtual void status(const QStringList& messageIds) = 0;
    virtual void balance() = 0;

    QString getLogin() const;
    void setLogin(const QString &value);

    QString getPassword() const;
    void setPassword(const QString &value);

    QString getSenderName() const;
    void setSenderName(const QString &value);

signals:
    void sendedIds(const QHash<QString,QString> idToRecepientHash);
    void smsStatus(const QHash<QString,QString> idToStatusHash);
    void balance(const double& currency);
    void error(const QString& error);

private:
    QString login;
    QString password;
    QString senderName;
};
#endif // ABSTRACTSMSGATEWAY_H

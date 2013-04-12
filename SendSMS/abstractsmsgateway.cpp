#include "abstractsmsgateway.h"

QString AbstractSMSGateway::getSenderName() const
{
    return senderName;
}

void AbstractSMSGateway::setSenderName(const QString &value)
{
    senderName = value;
}

QString AbstractSMSGateway::getPassword() const
{
    return password;
}

void AbstractSMSGateway::setPassword(const QString &value)
{
    password = value;
}

QString AbstractSMSGateway::getLogin() const
{
    return login;
}

void AbstractSMSGateway::setLogin(const QString &value)
{
    login = value;
}


AbstractSMSGateway::AbstractSMSGateway(QObject *parent) :
    QObject(parent)
{
}

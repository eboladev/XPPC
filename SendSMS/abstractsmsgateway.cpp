#include "abstractsmsgateway.h"

bool AbstractSMSGateway::getEnabled() const
{
    return enabled;
}

void AbstractSMSGateway::setEnabled(bool value)
{
    enabled = value;
}

QString AbstractSMSGateway::getClassId() const
{
    return classId;
}

void AbstractSMSGateway::setClassId(const QString &value)
{
    classId = value;
}

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

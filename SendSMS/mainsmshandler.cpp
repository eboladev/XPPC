#include "mainsmshandler.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QDebug>
#include <QCryptographicHash>
#include <QNetworkReply>

MainSMSHandler::MainSMSHandler(QObject *parent) :
    AbstractSMSGateway(parent)
{
    qnam = new QNetworkAccessManager(this);
}

void MainSMSHandler::send(const QString &smsText, const QStringList &recipientsList)
{        
    QStringList params;

    QStringList temp = recipientsList;
    temp.sort();
    params << formParam("project",getLogin())
           << formParam("sender",getSenderName())
           << formParam("message",smsText)
           << formParam("recipients",temp.join(","));
    if(testMode)
        params  << formParam("test","1")
                << formParam("sign",formSignature(QStringList() << smsText << temp.join(",") << "1" <<  getSenderName()));
    else
        params << formParam("sign",formSignature(QStringList() << smsText << temp.join(",") <<  getSenderName()));


    request(formUrl("http://mainsms.ru/api/mainsms/message/send?",params.join("&")));
}

void MainSMSHandler::status(const QStringList &messageIds)
{
    QStringList params;

    QStringList temp = messageIds;
    temp.sort();

    params << formParam("project",getLogin())
           << formParam("messages_id",temp.join(","))
           << formParam("sign",formSignature(QStringList() << temp.join(",")));

    request(formUrl("http://mainsms.ru/api/mainsms/message/status?",params.join("&")));
}

void MainSMSHandler::price(const QString &smsText, const QStringList &recipientsList)
{
    QStringList params;

    QStringList temp = recipientsList;
    temp.sort();

    params << formParam("project",getLogin())
           << formParam("message",smsText)
           << formParam("recipients",temp.join(","))
           << formParam("sign",formSignature(QStringList() << smsText << temp.join(",")));

    request(formUrl("http://mainsms.ru/api/mainsms/message/price?",params.join("&")));
}

void MainSMSHandler::balance()
{
    QStringList params;

    params << formParam("project",getLogin())
           << formParam("sign",formSignature());

    request(formUrl("http://mainsms.ru/api/mainsms/message/balance?",params.join("&")));
}

void MainSMSHandler::httpFinished()
{
    qobject_cast<QNetworkReply*>(sender())->deleteLater();
}

void MainSMSHandler::httpReadyRead()
{
    parseAnswer(qobject_cast<QNetworkReply*>(sender())->readAll());
}

bool MainSMSHandler::getTestMode() const
{
    return testMode;
}

void MainSMSHandler::setTestMode(bool value)
{
    testMode = value;
}

QString MainSMSHandler::formSignature(QStringList params)
{
    QStringList temp;
    temp << params << getLogin();
    temp.sort();
    return QCryptographicHash::hash(
                QCryptographicHash::hash(
                    QString(temp.join(";").append(";").append(getPassword())).toUtf8(),
                    QCryptographicHash::Sha1)
                .toHex(),QCryptographicHash::Md5).toHex();
}

QString MainSMSHandler::formParam(const QString &key, const QString &value)
{
    return QString("%0=%1").arg(key).arg(value);
}

QUrl MainSMSHandler::formUrl(const QString &path, const QString &params)
{
    return QUrl(QString().append(path).append(params));
}

void MainSMSHandler::request(const QUrl &url)
{
    qDebug() << url;
    QNetworkReply* reply = qnam->get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()),
            this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));
}

void MainSMSHandler::parseAnswer(const QString &answerReply)
{
    emit answer(answerReply);
    qDebug() << answerReply;
    //qDebug() << answer.indexOf(",");
    QString temp = answerReply;
    temp.remove(0,1);
    temp.remove(temp.length()-1,1);
    temp.remove("\"");
    // qDebug() << temp;
    //QMap<QString,QString> parsedAnswer;
    /*QStringList tempList = temp.split(QRegExp("(\\w+:.+,)+"));

    foreach (QString parsedString, tempList)
    {
        QString key = parsedString.left(parsedString.indexOf(":"));
        QString value = parsedString.right(parsedString.length() - 1 - parsedString.indexOf(":"));
        qDebug() << key << value << parsedString << parsedString.indexOf(":");
        parsedAnswer[key] = value;
    }*/
    // qDebug() << parsedAnswer;
}

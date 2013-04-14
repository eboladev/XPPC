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
    testMode = true;
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


    request(formUrl("http://mainsms.ru/api/mainsms/message/send?",params.join("&")),1);
}

void MainSMSHandler::status(const QStringList &messageIds)
{
    QStringList params;

    QStringList temp = messageIds;
    temp.sort();

    params << formParam("project",getLogin())
           << formParam("messages_id",temp.join(","))
           << formParam("sign",formSignature(QStringList() << temp.join(",")));

    request(formUrl("http://mainsms.ru/api/mainsms/message/status?",params.join("&")),2);
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

    request(formUrl("http://mainsms.ru/api/mainsms/message/price?",params.join("&")),3);
}

void MainSMSHandler::balance()
{
    QStringList params;

    params << formParam("project",getLogin())
           << formParam("sign",formSignature());

    request(formUrl("http://mainsms.ru/api/mainsms/message/balance?",params.join("&")),4);
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

void MainSMSHandler::request(const QUrl &url, const int &type)
{
    qDebug() << url;
    QNetworkReply* reply = qnam->get(QNetworkRequest(url));
    reply->setProperty("type",type);
    connect(reply, SIGNAL(finished()),
            this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));
}

void MainSMSHandler::parseAnswer(const QString &answerReply)
{    
    emit answer(answerReply);
    QString tempAnswer = answerReply;
    if (tempAnswer.contains("\"status\":\"error\""))
    {
        //{"status":"error","error":"4","message":"param recipients is blank"}
        tempAnswer.remove("\"");
        tempAnswer.remove("{");
        tempAnswer.remove("}");
        QRegExp re;
        int errorCode = 0;
        QString errorMessage;
        re.setPattern("(?:(error):(\\d+\\.*\\d*),*)");
        if (re.indexIn(tempAnswer) > -1)
            errorCode = re.cap(2).toInt();
        re.setPattern("(?:(message):(.+))");
        if (re.indexIn(tempAnswer) > -1)
            errorMessage = re.cap(2);

        emit error(errorCode, errorMessage);
    }
    switch( sender()->property("type").toInt())
    {
    case 1:
    {
        //status:success,recipients:[79657376072],messages_id:[0],count:1,parts:1,price:0.15,balance:2.7,test:1

        tempAnswer.remove("\"");
        tempAnswer.remove("{");
        tempAnswer.remove("}");

        QMap<QString,QString> parsedAnswer;
        while (!tempAnswer.isEmpty())
        {
            if (tempAnswer.indexOf(",") == -1)
            {
                putAnswerIntoHash(parsedAnswer,tempAnswer);
                break;
            }
            if ((tempAnswer.indexOf(",") < tempAnswer.indexOf("[")||(tempAnswer.indexOf("[") == -1)))
            {
                putAnswerIntoHash(parsedAnswer,tempAnswer.left(tempAnswer.indexOf(",")));
                tempAnswer.remove(0,tempAnswer.indexOf(",")+1);
            }
            else
            {
                putAnswerIntoHash(parsedAnswer,tempAnswer.left(tempAnswer.indexOf("]")+1));
                tempAnswer.remove(0,tempAnswer.indexOf("]")+2);
            }
        }

        QStringList recipients, messagesIds;
        QMapIterator<QString,QString> it(parsedAnswer);
        while(it.hasNext())
        {
            it.next();
            if (it.key() == "recipients")
            {
                QString recTemp = it.value();
                recTemp.remove("[");
                recTemp.remove("]");
                recipients << recTemp.split(",");
            }
            if (it.key() == "messages_id")
            {
                QString recTemp = it.value();
                recTemp.remove("[");
                recTemp.remove("]");
                messagesIds << recTemp.split(",");
            }
        }
        QHash<QString,QString> idToRecepientHash;
        while (!recipients.isEmpty())
            idToRecepientHash[recipients.takeFirst()] = messagesIds.takeFirst();

        emit sendedIds(idToRecepientHash);
    }
        break;
    case 2:
    {
        //{"status":"success","messages":{"54646":"not found","65654":"not found","79543":"not found"}}
        tempAnswer.remove("\"");
        tempAnswer.remove(0,26);
        tempAnswer.remove("}");
        QStringList temp = tempAnswer.split(",");
        QHash<QString,QString> idToStatusHash;
        while (!temp.isEmpty())
        {
            QString string = temp.takeFirst();
            QString key = string.left(string.indexOf(":"));
            QString value = string.right(string.length() - string.indexOf(":") - 1);
            idToStatusHash[key] = value;
        }
        emit smsStatus(idToStatusHash);
    }
        break;
    case 3:
    {
        //{"status":"success","recipients":["79657376072"],"count":1,"parts":1,"price":"0.0","balance":"1.65"}
        tempAnswer.remove("{");
        tempAnswer.remove("}");
        tempAnswer.remove("\"");
        int count,parts = 0;
        double price,balance = 0;
        QRegExp re;
        re.setPattern("(?:(count):(\\d+\\.*\\d*),*)");
        if (re.indexIn(tempAnswer) > -1)
            count = re.cap(2).toInt();
        re.setPattern("(?:(parts):(\\d+\\.*\\d*),*)");
        if (re.indexIn(tempAnswer) > -1)
            parts = re.cap(2).toInt();
        re.setPattern("(?:(price):(\\d+\\.*\\d*),*)");
        if (re.indexIn(tempAnswer) > -1)
            price = re.cap(2).toDouble();
        re.setPattern("(?:(balance):(\\d+\\.*\\d*),*)");
        if (re.indexIn(tempAnswer) > -1)
            balance = re.cap(2).toDouble();

        emit smsPrice(count,parts,price,balance);
    }
        break;
    case 4:
    {
        //{"status":"success","balance":"1.65"}
        tempAnswer.remove("{");
        tempAnswer.remove("}");
        tempAnswer.remove("\"");
        tempAnswer = tempAnswer.right(tempAnswer.indexOf(",")-2);
        tempAnswer = tempAnswer.right(tempAnswer.indexOf(":")-3);
        emit balanceCash(tempAnswer.toDouble());
    }
        break;
    default:
        break;
    }
}

void MainSMSHandler::putAnswerIntoHash(QMap<QString, QString> &answerHash, QString nonparsedString)
{
    QString key = nonparsedString.left(nonparsedString.indexOf(":"));
    QString value = nonparsedString.mid(nonparsedString.indexOf(":")+1);
    answerHash[key] = value;
}

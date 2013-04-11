#include "mainsmshandler.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QDebug>
#include <QStringList>
#include <QCryptographicHash>
#include <QUrl>
#include <QNetworkReply>

MainSMSHandler::MainSMSHandler(const QString &projectName, const QString &senderName, const QString &apiKey, QObject *parent) :
    QObject(parent),
    projectName(projectName),
    senderName(senderName),
    apiKey(apiKey)
{
    qnam = new QNetworkAccessManager(this);
}

void MainSMSHandler::send(const QString &smsText, const QStringList &recipientsList)
{
    QStringList params;

    //http://mainsms.ru/api/mainsms/message/send?project=mainsms&sender=mainsms.ru&message=test&recipients=89121231234&sign=12cb0a07a49b60222158b504bee88f57
    //http://mainsms.ru/api/mainsms/message/send?project=gradosh&sender=gradosh&message=hooe&recipinets=89657376072&sign=c4b32ddc6c70c82e2e3406892b2e5861"
    QString project = QString("project=%0").arg(projectName);
    QString sender = QString("sender=%0").arg(senderName);
    QString message = QString("message=%0").arg(smsText);

    QStringList temp = recipientsList;
    temp.sort();

    QString recipients = QString("recipients=%0").arg(temp.join(","));
    QString test = QString("test=%0").arg(1);// QString();//

    params << smsText << temp.join(",") << projectName << senderName << "1";
    params.sort();

    QString sign = QString("sign=%0").arg(formSignature(params.join(";")));
    temp = QStringList() << project << sender << message << recipients << test << sign;
    qDebug() << temp.join("&");
    QString req = temp.join("&");
    req.prepend("http://mainsms.ru/api/mainsms/message/send?");
    QUrl url = QUrl(req);
    qDebug() << url;
    QNetworkReply* reply = qnam->get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()),
            this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));
}

void MainSMSHandler::status(const QStringList &messageIds)
{
    //http://mainsms.ru/api/mainsms/message/status?project=mainsms&messages_id=1567,1568&sign=a2271659a8b05d318b4b53c644ba8b99
    QStringList temp = messageIds;
    temp.sort();
    QString project = QString("project=%0").arg(projectName);
    QString messages_id = QString("messages_id=%0").arg(temp.join(","));
    QStringList params;
    params << projectName << temp.join(",");
    QString sign = QString("sign=%0").arg(formSignature(params.join(";")));

    params = QStringList() << project << messages_id << sign;

    QString req = params.join("&");

    req.prepend("http://mainsms.ru/api/mainsms/message/status?");
    QUrl url = QUrl(req);
    qDebug() << url;
    QNetworkReply* reply = qnam->get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()),
            this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));
}

void MainSMSHandler::price(const QString &smsText, const QStringList &recipientsList)
{
    //http://mainsms.ru/api/mainsms/message/price?project=mainsms&message=test&recipients=89121231234,9121231235&sign=k2271659a8b05d318b4b53c644ba8b42
    QStringList temp = recipientsList;
    temp.sort();
    QString project = QString("project=%0").arg(projectName);
    QString message = QString("message=%0").arg(smsText);
    QString recipients = QString("recipients=%0").arg(temp.join(","));

    QStringList params;

    params << projectName << temp.join(",") << smsText;
    params.sort();

    qDebug() << params;

    QString sign = QString("sign=%0").arg(formSignature(params.join(";")));

    params = QStringList() << project << message << recipients << sign;

    QString req = params.join("&");

    req.prepend("http://mainsms.ru/api/mainsms/message/price?");
    QUrl url = QUrl(req);
    qDebug() << url;
    QNetworkReply* reply = qnam->get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()),
            this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));
}

void MainSMSHandler::balance()
{
    //http://mainsms.ru/api/mainsms/message/balance?project=mainsms&sign=y2271659a8b05d318b4b53c644ba8b0
    QStringList params = QStringList() << projectName;

    QString project = QString("project=%0").arg(projectName);
    QString sign = QString("sign=%0").arg(formSignature(params.join(";")));

    params = QStringList() << project << sign;

    QString req = params.join("&");

    req.prepend("http://mainsms.ru/api/mainsms/message/balance?");

    QUrl url = QUrl(req);
    qDebug() << url;
    QNetworkReply* reply = qnam->get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()),
            this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));
}

void MainSMSHandler::httpFinished()
{
    qobject_cast<QNetworkReply*>(sender())->deleteLater();
}

void MainSMSHandler::httpReadyRead()
{
    parseAnswer(qobject_cast<QNetworkReply*>(sender())->readAll());
}

QString MainSMSHandler::formSignature(const QString params)
{
    QString temp = params;
    temp.append(";").append(apiKey);
    qDebug() << params << temp;
    return QCryptographicHash::hash(
                QCryptographicHash::hash(
                    QString(temp).toUtf8(),
                    QCryptographicHash::Sha1)
                .toHex(),QCryptographicHash::Md5).toHex();
}

void MainSMSHandler::parseAnswer(const QString &answer)
{
    qDebug() << answer;
    //qDebug() << answer.indexOf(",");
    QString temp = answer;
    temp.remove(0,1);
    temp.remove(temp.length()-1,1);
    temp.remove("\"");
    qDebug() << temp;
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

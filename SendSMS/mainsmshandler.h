#ifndef MAINSMSHANDLER_H
#define MAINSMSHANDLER_H

#include <abstractsmsgateway.h>
#include <QUrl>
#include <QStringList>

class QNetworkAccessManager;
class QNetworkReply;

class MainSMSHandler : public AbstractSMSGateway
{
    Q_OBJECT
public:
    explicit MainSMSHandler(QObject *parent = 0);

    void send(const QString& smsText,
              const QStringList& recipientsList);
    void status(const QStringList& messageIds);
    void balance();
/*mainsms metohds*/
    void price(const QString& smsText,
               const QStringList& recipientsList);


    bool getTestMode() const;
    void setTestMode(bool value);

signals:
    void answer(const QString& text);

private slots:
    void httpFinished();
    void httpReadyRead();

private:
    QString formSignature(QStringList params = QStringList());
    QString formParam(const QString& key,
                      const QString& value);
    QUrl formUrl(const QString& path,
                 const QString& params);
    void request(const QUrl& url,const int& type);

    void parseAnswer(const QString& answerReply);
    void putAnswerIntoHash(QMap<QString, QString> &answerHash, QString nonparsedString);
private:
    QNetworkAccessManager* qnam;
    bool testMode;
};

#endif // MAINSMSHANDLER_H

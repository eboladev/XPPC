#ifndef MAINSMSHANDLER_H
#define MAINSMSHANDLER_H

#include <QObject>
#include <QMap>

class QNetworkAccessManager;
class QNetworkReply;

class MainSMSHandler : public QObject
{
    Q_OBJECT
public:
    explicit MainSMSHandler(const QString& projectName,
                            const QString& senderName,
                            const QString& apiKey,
                            QObject *parent = 0);

    void send(const QString& smsText,
              const QStringList& recipientsList);
    void status(const QStringList& messageIds);
    void price(const QString& smsText,
               const QStringList& recipientsList);
    void balance();

    void setProjectName(const QString& projectName);
    void setSenderName(const QString& senderName);
    void setApiKey(const QString& apiKey);

signals:
    void sendAnswer(const QMap<QString,QString> messageIdToRecepientMap,
                    const int& partsCount,
                    const double& sendPrice,
                    const double& currency);

    void statusAnswer(const QMap<QString,QString> messageIdToStatusMap);

    void priceAnswer(const QStringList& recipientsList,
                     const int& partsCount,
                     const double& sendPrice,
                     const double& currency);

    void error(const QString& status,
               const int& errorCode,
               const QString& errorMessage);

    void balance(const double& currency);

private slots:
    void httpFinished();
    void httpReadyRead();

private:
    QString formSignature(const QString params);

    void parseAnswer(const QString& answer);

private:
    QString projectName;
    QString senderName;
    QString apiKey;
    QNetworkAccessManager* qnam;

};

#endif // MAINSMSHANDLER_H

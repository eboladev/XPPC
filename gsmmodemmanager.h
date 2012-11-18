#ifndef GSMMODEMMANAGER_H
#define GSMMODEMMANAGER_H

#include <QDialog>
#include <QStandardItemModel>
#include "sqlextension.h"
class SendSMS;
namespace Ui {
class GsmModemManager;
}

class ItemModelForGSMModemManagement : public QStandardItemModel, SqlExtension
{
    Q_OBJECT
public:
    explicit ItemModelForGSMModemManagement(const QString &dbConnectionString, QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    void refreshModel();
    void addModem(QString modemName, const QString &portName, const bool &isActive);
    void deleteModem(const QModelIndex &index);

private slots:
    void onItemChanged (QStandardItem *item);

signals:
    void sameModemNameIsPresent(const QString &modemName);

signals:

public slots:

};

class GsmModemManager : public QDialog, SqlExtension
{
    Q_OBJECT
    
public:
    explicit GsmModemManager(const QString &dbConnectionString,QWidget *parent = 0);
    ~GsmModemManager();
private slots:
    void onAddButtonPushed();
    void onRemoveButtonPushed();
    void onCustomContextMenuRequested(const QPoint &pos);
    void onDeleteModem(const QModelIndex &index);
    void onSameModemNameIsPresent(const QString &modemName);

    void onSmsDelivered(QString modemName, int idSMS, bool delivered);
    void onSmsSended(QString modemName, int idSMS);
    void onStartSendingSMS(QString modemName, int idSMS);
    void onErrorWhileSendingSMS(QString modemName, int idSMS);
    void onIncomingRing(QString modemName, QString telNumber);
    void onIncomingSMS(QString modemName, QString phoneNumber, QString smsTest);
    void onSignalLevels(QString modemName, int rssi, int bor);
    void onPortStateChanged(QString modemName, bool isOpen);

    void onIsModemResponse(QString modemName, bool response);
    void onPortIsLost(QString modemName);
    void onPortIsRestored(QString modemName);
    void onCMEError(QString modemName, int errorNum);
    void onCMSError(QString modemName, int errorNum);
    void onError(QString modemName, QString lastCommand);
    void onIsRegisteringInNetwork(QString modemName, int status);
    void onOperatorName(QString modemName, QString operatorN);
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::GsmModemManager *ui;
    ItemModelForGSMModemManagement *model;
    SendSMS *sendSMS;
};

#endif // GSMMODEMMANAGER_H

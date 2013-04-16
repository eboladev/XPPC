#include "smsgatewaysettings.h"
#include "ui_smsgatewaysettings.h"

#include <QSettings>
#include <QDebug>

#include "smsmanager.h"

SmsGatewaySettings::SmsGatewaySettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SmsGatewaySettings)
{
    ui->setupUi(this);
    getCredentials(MainSMS);
    setAttribute(Qt::WA_DeleteOnClose);
    connect(ui->pushButtonOk, SIGNAL(clicked()), this, SLOT(onAccept()));
    connect(ui->pushButtonCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

SmsGatewaySettings::~SmsGatewaySettings()
{
    qDebug() << "here";
    delete ui;
}

void SmsGatewaySettings::getCredentials(const SupportedGateways &type) const
{
    QSettings s;
    QString temp = QString("smsgateway/%0/").arg(type);
    ui->lineEditLogin->setText(s.value(temp.append("login")).toString());
    ui->lineEditPassword->setText(s.value(temp.append("password")).toString());
    ui->lineEditSenderName->setText(s.value(temp.append("sendername")).toString());
}

void SmsGatewaySettings::setCredentials( const SupportedGateways &type)
{
    QSettings s;
    QString temp = QString("smsgateway/%0/").arg(type);
    s.setValue(temp.append("login"),ui->lineEditLogin->text().trimmed());
    s.setValue(temp.append("password"),ui->lineEditPassword->text().trimmed());
    s.setValue(temp.append("sendername"),ui->lineEditSenderName->text().trimmed());
}

void SmsGatewaySettings::onAccept()
{
    setCredentials(MainSMS);
    smsManager->loadGsmGatewaySettings();
    accept();
}

#include "smtpsettingsdialog.h"
#include "ui_smtpsettingsdialog.h"

SmtpSettingsDialog::SmtpSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SmtpSettingsDialog)
{
    ui->setupUi(this);
    connect(ui->pushButtonOk, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->pushButtonCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

SmtpSettingsDialog::~SmtpSettingsDialog()
{
    delete ui;
}

void SmtpSettingsDialog::setSMTPAddress(const QString &address)
{
    ui->lineEditSMTPAddress->setText(address);
}

void SmtpSettingsDialog::setSMTPPort(const int &port)
{
    ui->spinBoxSMTPPort->setValue(port);
}

void SmtpSettingsDialog::setSMTPLogin(const QString &login)
{
    ui->lineEditSMTPLogin->setText(login);
}

void SmtpSettingsDialog::setSMTPPass(const QString &pass)
{
    ui->lineEditSMTPPass->setText(pass);
}

void SmtpSettingsDialog::setSMTPSenderName(const QString &sender_name)
{
    ui->lineEditSMTPSenderName->setText(sender_name);
}

void SmtpSettingsDialog::setSMTPSenderAddress(const QString &sender_address)
{
    ui->lineEditSMTPSenderAddress->setText(sender_address);
}

void SmtpSettingsDialog::setMailToAddress(const QString &mailTo_address)
{
    ui->lineEditMailTo->setText(mailTo_address);
}

QString SmtpSettingsDialog::getSMTPAddress() const
{
    return ui->lineEditSMTPAddress->text().trimmed();
}

int SmtpSettingsDialog::getSMTPPort() const
{
    return ui->spinBoxSMTPPort->value();
}

QString SmtpSettingsDialog::getSMTPLogin() const
{
    return ui->lineEditSMTPLogin->text().trimmed();
}

QString SmtpSettingsDialog::getSMTPPass() const
{
    return ui->lineEditSMTPPass->text().trimmed();
}

QString SmtpSettingsDialog::getSMTPSenderName() const
{
    return ui->lineEditSMTPSenderName->text().trimmed();
}

QString SmtpSettingsDialog::getSMTPSenderAddress() const
{
    return ui->lineEditSMTPSenderAddress->text().trimmed();
}

QString SmtpSettingsDialog::getMailToAddress() const
{
    return ui->lineEditMailTo->text().trimmed();
}

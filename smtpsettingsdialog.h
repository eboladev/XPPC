#ifndef SMTPSETTINGSDIALOG_H
#define SMTPSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SmtpSettingsDialog;
}

class SmtpSettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SmtpSettingsDialog(QWidget *parent = 0);
    ~SmtpSettingsDialog();
    void setSMTPAddress(const QString& address);
    void setSMTPPort(const int& port);
    void setSMTPLogin(const QString& login);
    void setSMTPPass(const QString& pass);
    void setSMTPSenderName(const QString& sender_name);
    void setSMTPSenderAddress(const QString& sender_address);
    void setMailToAddress(const QString& mailTo_address);

    QString getSMTPAddress() const;
    int getSMTPPort() const;
    QString getSMTPLogin() const;
    QString getSMTPPass() const;
    QString getSMTPSenderName() const;
    QString getSMTPSenderAddress() const;
    QString getMailToAddress() const;

private:
    Ui::SmtpSettingsDialog *ui;
};

#endif // SMTPSETTINGSDIALOG_H

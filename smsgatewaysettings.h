#ifndef SMSGATEWAYSETTINGS_H
#define SMSGATEWAYSETTINGS_H

#include <QDialog>

namespace Ui {
class SmsGatewaySettings;
}

enum SupportedGateways
{
    MainSMS = 0
};

class SmsGatewaySettings : public QDialog
{
    Q_OBJECT
    
public:
    explicit SmsGatewaySettings(QWidget *parent = 0);
    ~SmsGatewaySettings();

private:
    void getCredentials(const SupportedGateways& type) const;
    void setCredentials(const SupportedGateways& type);

private Q_SLOTS:
    void onAccept();
    
private:
    Ui::SmsGatewaySettings *ui;
};

#endif // SMSGATEWAYSETTINGS_H

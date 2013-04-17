#ifndef SMSTEMPLATESETTINGS_H
#define SMSTEMPLATESETTINGS_H

#include <QDialog>

namespace Ui {
class SmsTemplateSettings;
}

class SmsTemplateSettings : public QDialog
{
    Q_OBJECT
    
public:
    explicit SmsTemplateSettings(QWidget *parent = 0);
    ~SmsTemplateSettings();

private:
    void loadTemplate();

private Q_SLOTS:
    void onAccept();

private:
    Ui::SmsTemplateSettings *ui;
};

#endif // SMSTEMPLATESETTINGS_H

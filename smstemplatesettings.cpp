#include "smstemplatesettings.h"
#include "ui_smstemplatesettings.h"

#include <QSettings>

SmsTemplateSettings::SmsTemplateSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SmsTemplateSettings)
{
    ui->setupUi(this);
    connect(ui->pushButtonOk, SIGNAL(clicked()), this, SLOT(onAccept()));
    connect(ui->pushButtonCancel, SIGNAL(clicked()), this, SLOT(reject()));
    loadTemplate();
}

SmsTemplateSettings::~SmsTemplateSettings()
{
    delete ui;
}

void SmsTemplateSettings::loadTemplate()
{
    QSettings s;
    ui->plainTextEditTemplateText->setPlainText(s.value("smsTemplates/ticket").toString());
}

void SmsTemplateSettings::onAccept()
{
    QSettings s;
    s.setValue("smsTemplates/ticket",ui->plainTextEditTemplateText->toPlainText());
    accept();
}

#include "devicewidget.h"
#include "ui_devicewidget.h"

DeviceWidget::DeviceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceWidget)
{
    ui->setupUi(this);    
}

DeviceWidget::~DeviceWidget()
{
    delete ui;
}

void DeviceWidget::setDeviceName(const QString name)
{
    ui->lineEditName->setText(name);
}

void DeviceWidget::setDeviceCondition(const QString condition)
{
    ui->lineEditCondition->setText(condition);
}

void DeviceWidget::setDeviceSerialNumber(const QString serialNumber)
{
    ui->lineEditSerial->setText(serialNumber);
}

void DeviceWidget::setDeviceProblem(const QString text)
{
    ui->textEditProblem->setPlainText(text);
}

QString DeviceWidget::getDeviceName() const
{
    return ui->lineEditName->text().trimmed();
}

QString DeviceWidget::getDeviceCondition() const
{
    return ui->lineEditCondition->text().trimmed();
}

QString DeviceWidget::getDeviceSerialNumber() const
{
    return ui->lineEditSerial->text().trimmed();
}

QString DeviceWidget::getDeviceProblem() const
{
    return ui->textEditProblem->toPlainText().trimmed();
}

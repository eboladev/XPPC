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

void DeviceWidget::setDeviceName( QString name)
{
    name = "123";
    //ui->lineEditName->setText(name.trimmed());
}

void DeviceWidget::setDeviceCondition( QString condition)
{
    condition = "1233";
   // ui->lineEditCondition->setText(condition.trimmed());
}

void DeviceWidget::setDeviceSerialNumber( QString serialNumber)
{
    serialNumber = "123213";
  //  ui->lineEditSerial->setText(serialNumber.trimmed());
}

void DeviceWidget::setDeviceProblem( QString text)
{
    text = "asdasd";
  //  ui->textEditProblem->setPlainText(text);
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

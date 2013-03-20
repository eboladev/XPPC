#include "devicewidget.h"
#include "ui_devicewidget.h"

#include <QDebug>
#include <QStandardItemModel>

DeviceWidget::DeviceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceWidget)
{
    ui->setupUi(this);    
    deviceModel = new QStandardItemModel(this);
    ui->treeViewDeviceList->setModel(deviceModel);
    ui->groupBoxAddedDevices->setVisible(false);
    ui->treeViewDeviceList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    deviceModel->setHorizontalHeaderLabels(QStringList() << trUtf8("Название") << trUtf8("Состояние") << trUtf8("Серийный номер"));
    connect(ui->pushButtonAdd, SIGNAL(clicked()), SLOT(onAddDeviceClicked()));
    connect(ui->pushButtonRemove, SIGNAL(clicked()), SLOT(onRemoveDeviceClicked()));
    connect(ui->treeViewDeviceList->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(onCurrentChanged(QModelIndex,QModelIndex)));
    connect(ui->textEditProblem, SIGNAL(textChanged()), SLOT(onEnableAddDeviceButton()));
    connect(ui->lineEditCondition, SIGNAL(textChanged(QString)), SLOT(onEnableAddDeviceButton()));
    connect(ui->lineEditName, SIGNAL(textChanged(QString)), SLOT(onEnableAddDeviceButton()));
    connect(ui->lineEditSerial, SIGNAL(textChanged(QString)), SLOT(onEnableAddDeviceButton()));
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

void DeviceWidget::setDeviceControlVisible(const bool &visible)
{
    ui->pushButtonAdd->setVisible(visible);
    ui->pushButtonRemove->setVisible(visible);
}

void DeviceWidget::onAddDeviceClicked()
{
    QStandardItem* dName = new QStandardItem(getDeviceName());
    dName->setData(getDeviceProblem());
    QStandardItem* dCondition = new QStandardItem(getDeviceCondition());
    QStandardItem* dSerial = new QStandardItem(getDeviceSerialNumber());
    deviceModel->appendRow(QList<QStandardItem*>() << dName << dCondition << dSerial);
    ui->groupBoxAddedDevices->setVisible(true);
    ui->pushButtonAdd->setEnabled(false);
}

void DeviceWidget::onRemoveDeviceClicked()
{
    if (ui->treeViewDeviceList->currentIndex().isValid())
        deviceModel->removeRow(ui->treeViewDeviceList->currentIndex().row());
    if (deviceModel->rowCount() == 0)
        ui->groupBoxAddedDevices->setVisible(false);
}

void DeviceWidget::onCurrentChanged(QModelIndex current, QModelIndex)
{
    ui->pushButtonRemove->setEnabled(current.isValid() ? true : false);
}

void DeviceWidget::onEnableAddDeviceButton()
{
    if (getDeviceName().isEmpty())
        return;
    ui->pushButtonAdd->setEnabled(true);
}

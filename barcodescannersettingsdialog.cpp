#include "barcodescannersettingsdialog.h"
#include "ui_barcodescannersettingsdialog.h"

#include <QSettings>
#include <QMessageBox>
#include <QSerialPortInfo>

#include "barcodehandler.h"

BarcodeScannerSettingsDialog::BarcodeScannerSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BarcodeScannerSettingsDialog)
{
    ui->setupUi(this);
    ui->comboBoxAvailablePorts->addItems(barcodeHandler->getAvailablePorts());
    ui->comboBoxBaudRate->addItems(barcodeHandler->getStandardBaudRates());
    connect(ui->pushButtonOk, SIGNAL(clicked()), this, SLOT(onAccept()));
    connect(ui->pushButtonCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui->pushButtonCheckSettings, SIGNAL(clicked()), this, SLOT(onCheckSettings()));
    loadSettigns();
}

BarcodeScannerSettingsDialog::~BarcodeScannerSettingsDialog()
{
    delete ui;
}

void BarcodeScannerSettingsDialog::onAccept()
{
    QSettings s;
    s.setValue("barcodeScanner/port",ui->comboBoxAvailablePorts->currentText());
    s.setValue("barcodeScanner/baudRate",ui->comboBoxBaudRate->currentText());
    s.setValue("barcodeScanner/enabled",ui->groupBoxPortSettings->isChecked());
    s.setValue("barcodeScanner/DataBits", ui->comboBoxDataBits->currentText());
    s.setValue("barcodeScanner/FlowControl", ui->comboBoxFlowControl->currentText());
    s.setValue("barcodeScanner/StopBits",ui->comboBoxStopBits->currentText());
    s.setValue("barcodeScanner/Parity", ui->comboBoxParity->currentText());
    if (barcodeHandler->initBarcodeScanner())
        accept();
    else
        QMessageBox::critical(0, QObject::trUtf8("Ошибка"),
                              QObject::trUtf8("Сканер штрих кода не инициализирован, проверьте настройки"));
}

void BarcodeScannerSettingsDialog::onCheckSettings()
{
    QSettings s;
    s.setValue("barcodeScanner/port",ui->comboBoxAvailablePorts->currentText());
    s.setValue("barcodeScanner/baudRate",ui->comboBoxBaudRate->currentText());
    s.setValue("barcodeScanner/enabled",ui->groupBoxPortSettings->isChecked());
    s.setValue("barcodeScanner/DataBits", ui->comboBoxDataBits->currentText());
    s.setValue("barcodeScanner/FlowControl", ui->comboBoxFlowControl->currentText());
    s.setValue("barcodeScanner/StopBits",ui->comboBoxStopBits->currentText());
    s.setValue("barcodeScanner/Parity", ui->comboBoxParity->currentText());
    if (barcodeHandler->initBarcodeScanner())
    {
        QSerialPortInfo qspi = QSerialPortInfo(barcodeHandler->getPort());
        ui->labelDescription->setText(qspi.description());
        ui->labelManufacturer->setText(qspi.manufacturer());        
        //ui->labelProductId->setText(QString::number(qspi.productIdentifier()));
        //ui->labelVendorId->setText(QString::number(qspi.vendorIdentifier()));
    }
}

void BarcodeScannerSettingsDialog::loadSettigns()
{
    QSettings s;
    QString port = s.value("barcodeScanner/port").toString();
    ui->comboBoxAvailablePorts->setCurrentIndex(ui->comboBoxAvailablePorts->findText(port));
    QString baud = s.value("barcodeScanner/baudRate").toString();
    ui->comboBoxBaudRate->setCurrentIndex(ui->comboBoxBaudRate->findText(baud));
    bool enabled = s.value("barcodeScanner/enabled").toBool();
    ui->groupBoxPortSettings->setChecked(enabled);
    QString dataBits = s.value("barcodeScanner/DataBits").toString();
    QString flowControl = s.value("barcodeScanner/FlowControl").toString();
    QString stopBits = s.value("barcodeScanner/StopBits").toString();
    QString parity = s.value("barcodeScanner/Parity").toString();
    ui->comboBoxDataBits->setCurrentIndex(ui->comboBoxDataBits->findText(dataBits));
    ui->comboBoxFlowControl->setCurrentIndex(ui->comboBoxFlowControl->findText(flowControl));
    ui->comboBoxStopBits->setCurrentIndex(ui->comboBoxStopBits->findText(stopBits));
    ui->comboBoxParity->setCurrentIndex(ui->comboBoxParity->findText(parity));
}

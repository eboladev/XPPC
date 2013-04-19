#include "barcodehandler.h"

#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QStringList>
#include <QSettings>

BarcodeHandler::BarcodeHandler(QObject *parent) :
    QObject(parent)
{
    serialPort = new QSerialPort();
    connect(serialPort, SIGNAL(readyRead()),
            this,       SLOT(onReadyRead()));
}

Q_GLOBAL_STATIC(BarcodeHandler, singlton_instance)
BarcodeHandler *BarcodeHandler::instance()
{
    return singlton_instance();
}

bool BarcodeHandler::initBarcodeScanner()
{
    QSettings s;
    bool enabled = s.value("barcodeScanner/enabled").toBool();
    if (!enabled)
        return false;
    QString port = s.value("barcodeScanner/port").toString();    
    QSerialPort::BaudRate baudRate = getBaudRate(s.value("barcodeScanner/baudRate").toInt());
    QSerialPort::DataBits bits = getDataBits(s.value("barcodeScanner/DataBits").toInt());
    QSerialPort::FlowControl flowControl = getFlowControl(s.value("barcodeScanner/FlowControl").toString());
    QSerialPort::Parity parity = getParity(s.value("barcodeScanner/Parity").toString());
    QSerialPort::StopBits stopBits = getStopBits(s.value("barcodeScanner/StopBits").toDouble());

    if (serialPort->isOpen())
        serialPort->close();

    serialPort->setPort(QSerialPortInfo(port));

    if (serialPort->open(QSerialPort::ReadOnly))
    {
        serialPort->setDataBits(bits);
        serialPort->setFlowControl(flowControl);
        serialPort->setParity(parity);
        serialPort->setStopBits(stopBits);
        serialPort->setBaudRate(baudRate);
        return true;
    }
    return false;
}

QStringList BarcodeHandler::getAvailablePorts()
{
    QList<QSerialPortInfo> si = QSerialPortInfo::availablePorts();
    QStringList portNames;
    foreach(QSerialPortInfo s, si)
        portNames << s.portName();
    return portNames;
}

QStringList BarcodeHandler::getStandardBaudRates()
{
    QStringList rates;
    foreach (quint32 rate, QSerialPortInfo::standardBaudRates()) {
        rates << QString::number(rate);
    }
    return rates;
}

QString BarcodeHandler::getPort()
{
    return serialPort->portName();
}

void BarcodeHandler::onReadyRead()
{
    QByteArray arr = serialPort->readAll();
    emit barcodeRead(QString::fromAscii(arr.data(),arr.length()).replace("\n",""));
}

PortSettings BarcodeHandler::loadSettings()
{
    PortSettings ps;
    return ps;
}

QSerialPort::DataBits BarcodeHandler::getDataBits(const int &dataBits)
{
    switch(dataBits)
    {
    case 5:
        return QSerialPort::Data5;
        break;
    case 6:
        return QSerialPort::Data6;
        break;
    case 7:
        return QSerialPort::Data7;
        break;
    case 8:
        return QSerialPort::Data8;
        break;
    default:
        return QSerialPort::Data8;
        break;
    }
}

QSerialPort::BaudRate BarcodeHandler::getBaudRate(const int &rate)
{
    switch (rate) {
    case 1200:
        return QSerialPort::Baud1200;
        break;
    case 2400:
        return QSerialPort::Baud2400;
        break;
    case 4800:
        return QSerialPort::Baud4800;
        break;
    case 9600:
        return QSerialPort::Baud9600;
        break;
    case 19200:
        return QSerialPort::Baud19200;
        break;
    case 57600:
        return QSerialPort::Baud57600;
        break;
    case 115200:
        return QSerialPort::Baud115200;
        break;
    default:
        return QSerialPort::Baud9600;
        break;
    }
}

QSerialPort::FlowControl BarcodeHandler::getFlowControl(const QString &value)
{
    if (value == trUtf8("Нет"))
        return QSerialPort::NoFlowControl;
    if (value == trUtf8("Программно"))
        return QSerialPort::SoftwareControl;
    if (value == trUtf8("Аппаратно"))
        return QSerialPort::HardwareControl;
    return QSerialPort::NoFlowControl;
}

QSerialPort::Parity BarcodeHandler::getParity(const QString &value)
{
    if (value == trUtf8("Нет"))
        return QSerialPort::NoParity;
    if (value == trUtf8("Чет"))
        return QSerialPort::EvenParity;
    if (value == trUtf8("Нечет"))
        return QSerialPort::OddParity;
    if (value == trUtf8("Маркер"))
        return QSerialPort::MarkParity;
    if (value == trUtf8("Пробел"))
        return QSerialPort::SpaceParity;

    return QSerialPort::NoParity;
}

QSerialPort::StopBits BarcodeHandler::getStopBits(const double &bits)
{
    if (bits == 1)
        return QSerialPort::OneStop;
    if (bits == 1.5)
        return QSerialPort::OneAndHalfStop;
    if (bits == 2)
        return QSerialPort::TwoStop;
    return QSerialPort::OneStop;
}

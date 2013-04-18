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
    QString port = s.value("barcodeScanner/port").toString();
    QString baud = s.value("barcodeScanner/baudRate").toString();
    QSerialPort::BaudRate baudRate;
    switch (baud.toInt()) {
    case 1200:
        baudRate = QSerialPort::Baud1200;
        break;
    case 2400:
        baudRate = QSerialPort::Baud2400;
        break;
    case 4800:
        baudRate = QSerialPort::Baud4800;
        break;
    case 9600:
        baudRate = QSerialPort::Baud9600;
        break;
    case 19200:
        baudRate = QSerialPort::Baud19200;
        break;
    case 57600:
        baudRate = QSerialPort::Baud57600;
        break;
    case 115200:
        baudRate = QSerialPort::Baud115200;
        break;
    default:
        baudRate = QSerialPort::Baud9600;
        break;
    }

    if (serialPort->isOpen())
        serialPort->close();

    serialPort->setPort(QSerialPortInfo(port));

    if (serialPort->open(QSerialPort::ReadOnly))
    {
        serialPort->setDataBits(QSerialPort::Data8);
        serialPort->setFlowControl(QSerialPort::NoFlowControl);
        serialPort->setParity(QSerialPort::NoParity);
        serialPort->setStopBits(QSerialPort::OneStop);
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

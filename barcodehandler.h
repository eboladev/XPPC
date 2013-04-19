#ifndef BARCODEHANDLER_H
#define BARCODEHANDLER_H

#include <QObject>
#include <QtSerialPort/QSerialPort>

#define barcodeHandler (static_cast<BarcodeHandler *>(BarcodeHandler::instance()))

class QSerialPort;

struct PortSettings
{
    QString portName;
    QSerialPort::BaudRate baudRate = QSerialPort::Baud9600;
    QSerialPort::DataBits dataBits = QSerialPort::Data8;
    QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl;
    QSerialPort::Parity parity = QSerialPort::NoParity;
    QSerialPort::StopBits stopBits = QSerialPort::OneStop;
};

class BarcodeHandler : public QObject
{
    Q_OBJECT
public:
    explicit BarcodeHandler(QObject *parent = 0);
    static BarcodeHandler* instance();

    bool initBarcodeScanner();
    QStringList getAvailablePorts();
    QStringList getStandardBaudRates();
    QString getPort();

Q_SIGNALS:
    void barcodeRead(const QString& barcode);

private Q_SLOTS:
    void onReadyRead();
private:
    PortSettings loadSettings();
    QSerialPort::DataBits getDataBits(const int& dataBits);
    QSerialPort::BaudRate getBaudRate(const int& rate);
    QSerialPort::FlowControl getFlowControl(const QString& value);
    QSerialPort::Parity getParity(const QString& value);
    QSerialPort::StopBits getStopBits(const double& bits);

private:
    QSerialPort *serialPort;
};

#endif // BARCODEHANDLER_H

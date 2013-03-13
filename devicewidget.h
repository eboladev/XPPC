#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QWidget>

namespace Ui {
class DeviceWidget;
}

class DeviceWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit DeviceWidget(QWidget *parent = 0);
    ~DeviceWidget();
    void setDeviceName( QString name);
    void setDeviceCondition( QString condition);
    void setDeviceSerialNumber( QString serialNumber);
    void setDeviceProblem( QString text);
    QString getDeviceName() const;
    QString getDeviceCondition() const;
    QString getDeviceSerialNumber() const;
    QString getDeviceProblem() const;
    
private:
    Ui::DeviceWidget *ui;
};

#endif // DEVICEWIDGET_H

#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QWidget>
#include <QModelIndex>

class QStandardItemModel;

namespace Ui {
class DeviceWidget;
}

class DeviceWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit DeviceWidget(QWidget *parent = 0);
    ~DeviceWidget();
    void setDeviceName(const QString name);
    void setDeviceCondition(const QString condition);
    void setDeviceSerialNumber(const QString serialNumber);
    void setDeviceProblem(const QString text);
    QString getDeviceName() const;
    QString getDeviceCondition() const;
    QString getDeviceSerialNumber() const;
    QString getDeviceProblem() const;
    void setDeviceControlVisible(const bool& visible = false);
    QStandardItemModel* deviceModel;

private slots:
    void onAddDeviceClicked();
    void onRemoveDeviceClicked();
    void onCurrentChanged(QModelIndex, QModelIndex);
    void onEnableAddDeviceButton();

private:
    Ui::DeviceWidget *ui;    
};

#endif // DEVICEWIDGET_H

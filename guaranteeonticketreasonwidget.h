#ifndef GUARANTEEONTICKETREASONWIDGET_H
#define GUARANTEEONTICKETREASONWIDGET_H

#include <QDialog>
#include <QHash>
#include <QModelIndex>
namespace Ui {
class GuaranteeOnTicketReasonWidget;
}
class QStandardItemModel;
class GuaranteeOnTicketReasonWidget : public QDialog
{
    Q_OBJECT
    
public:
    explicit GuaranteeOnTicketReasonWidget(QWidget *parent = 0);
    ~GuaranteeOnTicketReasonWidget();
    QHash<QString,QString> ticketGuaranteeText;
    void addDevice(const QVariant& id, const int& ticket_id, const QString& name, const QString& serial);

private Q_SLOTS:
    void onCurrentTicketChanged(QModelIndex,QModelIndex);
    void onAccept();

private:
    Ui::GuaranteeOnTicketReasonWidget *ui;
    QStandardItemModel* model;
    QVariant currentId;
};

#endif // GUARANTEEONTICKETREASONWIDGET_H

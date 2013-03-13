#ifndef RECEIPTMANAGER_H
#define RECEIPTMANAGER_H

#include <QDialog>
#include "sqlextension.h"

class QStringListModel;
class CustomerWidget;
class DeviceWidget;

namespace Ui {
class ReceiptManager;
}

class ReceiptManager : public QDialog, SqlExtension
{
    Q_OBJECT
    
public:
    explicit ReceiptManager(const QString dbConnectionsString, QWidget *parent = 0);
    explicit ReceiptManager(const QString dbConnectionsString, const int id, QWidget *parent = 0);
    ~ReceiptManager();
    
private slots:
    void on_pushButtonAddReceipt_clicked();

    void on_pushButtonClearFields_clicked();

    void onFIOTextChanged(QString);

private:
    void setupConnections();
    void initWidgets();
    void fillBranchComboBox();

private:
    Ui::ReceiptManager *ui;
    void clearFields();
    void fillFields(int id);
    int branch;
    QStringList wordList;    
    QStringListModel* model;
    CustomerWidget* cw;
    DeviceWidget* dw;
};

#endif // RECEIPTMANAGER_H

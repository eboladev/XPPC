#ifndef RECEIPTMANAGER_H
#define RECEIPTMANAGER_H

#include <QDialog>
#include "sqlextension.h"
class QStringListModel;
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
    void setupConnects();
private:
    Ui::ReceiptManager *ui;
    void clearFields();
    void fillFields(int id);
    int branch;
    QStringList wordList;
    QStringListModel* model;
};

#endif // RECEIPTMANAGER_H

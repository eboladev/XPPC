#ifndef RECEIPTMANAGER_H
#define RECEIPTMANAGER_H

#include <QDialog>
#include "sqlextension.h"



namespace Ui {
class ReceiptManager;
}

class ReceiptManager : public QDialog, SqlExtension
{
    Q_OBJECT
    
public:    
    explicit ReceiptManager(const QString dbConnectionsString, const int id = -1, QWidget *parent = 0);
    ~ReceiptManager();
    
private slots:
    void onAddReceiptClicked();

private:
    void setupConnections();
    void initWidgets();    

private:
    Ui::ReceiptManager *ui;
    void clearFields();
    void fillFields(int id);
    int branch;     
};

#endif // RECEIPTMANAGER_H

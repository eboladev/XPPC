#ifndef JOBLISTONRECEIPTDIALOG_H
#define JOBLISTONRECEIPTDIALOG_H

#include <QDialog>
#include "sqlextension.h"

namespace Ui {
class JobListOnReceiptDialog;
}

class JobListOnReceiptDialog : public QDialog, SqlExtension
{
    Q_OBJECT
    
public:
    explicit JobListOnReceiptDialog(const QString dbConnectionsString, const int id, QWidget *parent = 0);
    ~JobListOnReceiptDialog();
    
private slots:
    void on_comboBoxEmployeeList_currentIndexChanged(int index);

private:
    Ui::JobListOnReceiptDialog *ui;
    void getEmployeeList();
};

#endif // JOBLISTONRECEIPTDIALOG_H

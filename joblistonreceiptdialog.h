#ifndef JOBLISTONRECEIPTDIALOG_H
#define JOBLISTONRECEIPTDIALOG_H

#include <QDialog>
#include "sqlextension.h"

class QSqlQueryModel;

namespace Ui {
class JobListOnReceiptDialog;
}

class JobListOnReceiptDialog : public QDialog, SqlExtension
{
    Q_OBJECT
    
public:
    explicit JobListOnReceiptDialog(const QString dbConnectionsString, const int &id, QWidget *parent = 0);
    ~JobListOnReceiptDialog();
    
private slots:
    void on_comboBoxEmployeeList_currentIndexChanged(int index);

    void on_pushButtonClearField_clicked();

    void on_pushButtonAddJob_clicked();

    void on_pushButtonDeleteJob_clicked();

    void on_tableView_clicked(const QModelIndex &index);

private:
    void clearField();
    void getEmployeeList();
    void getJobs(const int &id);
private:
    Ui::JobListOnReceiptDialog *ui;    
    QSqlQueryModel* model;
    int m_id;
    int m_jid;
};

#endif // JOBLISTONRECEIPTDIALOG_H

#ifndef JOBLISTONRECEIPTDIALOG_H
#define JOBLISTONRECEIPTDIALOG_H

#include <QDialog>
#include "sqlextension.h"

class JobItemModel;
class EmployeeWidget;
class QSortFilterProxyModel;
class JobListItemModel;
class QCompleter;

namespace Ui {
class JobListOnReceiptDialog;
}

class JobListOnReceiptDialog : public QDialog, SqlExtension
{
    Q_OBJECT
    
public:
    explicit JobListOnReceiptDialog(const QString dbConnectionsString, const int &id,const int& ticket_id, QWidget *parent = 0);
    ~JobListOnReceiptDialog();
    
private Q_SLOTS:
    void clearField();

    void onPushButtonAddJobClicked();

    void onPushButtonDeleteJobClicked();

    void onUpdateClicked();
    void onCurrentSelectionChanged(QModelIndex, QModelIndex);

    void onCompleteJobData(QModelIndex);    

private:
    void getJobs(const QVariant &id);
    bool checkPermissions(const QVariant& selectedEmployeeId) const;
    QCompleter* getCompleter();

private:
    Ui::JobListOnReceiptDialog *ui;    
    JobItemModel* jobsModel;
    EmployeeWidget* employeeWidget;
    QSortFilterProxyModel* jobListProxy;
    JobListItemModel* jobListModel;
    int m_id;
    int m_jid;
};

#endif // JOBLISTONRECEIPTDIALOG_H

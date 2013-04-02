#ifndef USERMANAGEMENTDIALOG_H
#define USERMANAGEMENTDIALOG_H

#include <QDialog>

#include "sqlextension.h"

class EmployeeItemModel;
class QStandardItem;
class QSortFilterProxyModel;

namespace Ui {
class UserManagementDialog;
}

class UserManagementDialog : public QDialog, SqlExtension
{
    Q_OBJECT
    
public:
    explicit UserManagementDialog(const QString& dbConnectionString, QWidget *parent = 0);
    ~UserManagementDialog();

private slots:
    void onCustomContextMenuRequested(const QPoint&);
    void onCurrentEmployeeChanges(QModelIndex, QModelIndex);
    void onAddEmployee();
    void onFireEmployee();
    void refreshModel();
    void onChangeLoginpass();
    void onUserNameChanged(QString);
    void onUserInfoChangesSaved();

private:
    QStandardItem* getItemFromIndex(QModelIndex);

private:
    Ui::UserManagementDialog *ui;
    EmployeeItemModel* employeeModel;
    QSortFilterProxyModel* employeeProxyModel;
};

#endif // USERMANAGEMENTDIALOG_H

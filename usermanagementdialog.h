#ifndef USERMANAGEMENTDIALOG_H
#define USERMANAGEMENTDIALOG_H

#include <QDialog>

#include "sqlextension.h"

class EmployeeItemModel;
class QStandardItem;
class QSortFilterProxyModel;
class QStandardItemModel;
class UserInfoWidget;

namespace Ui {
class UserManagementDialog;
}

class UserManagementDialog : public QDialog, SqlExtension
{
    Q_OBJECT
    
public:
    explicit UserManagementDialog(const QString& dbConnectionString, QWidget *parent = 0);
    ~UserManagementDialog();

private Q_SLOTS:
    void onCustomContextMenuRequested(const QPoint&);
    void onCurrentEmployeeChanges(QModelIndex, QModelIndex);
    void onAddEmployee();
    void onFireEmployee();
    void refreshModel();
    void onChangeLoginpass();
    void onUserNameChanged(QString);
    void onUserInfoChangesSaved();

    void onEditGroupPermissions(const int& permissions);
    void onAddGroup();
    void onDeleteGroup();
    void onCurrentGroupChanged(QModelIndex, QModelIndex);
    void onCustomGroupsContextMenuRequested(const QPoint&);

private:
    QStandardItem* getItemFromIndex(QModelIndex);
    void refreshGroups();

Q_SIGNALS:
    void permissons(const int& value);

private:
    Ui::UserManagementDialog *ui;
    EmployeeItemModel* employeeModel;
    QVariant currentGroupId;
    QSortFilterProxyModel* employeeProxyModel;
    QString m_dbConnectionString;
    UserInfoWidget* uiw;
    QStandardItemModel* groupsModel;
};

#endif // USERMANAGEMENTDIALOG_H

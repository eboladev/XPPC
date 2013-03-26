#ifndef USERMANAGEMENTDIALOG_H
#define USERMANAGEMENTDIALOG_H

#include <QDialog>
#include <QStyledItemDelegate>

#include "sqlextension.h"

class EmployeeItemModel;
class QStandardItem;
class QSortFilterProxyModel;

namespace Ui {
class UserManagementDialog;
}

class CalendarDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    CalendarDelegate(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class UserManagementDialog : public QDialog, SqlExtension
{
    Q_OBJECT
    
public:
    explicit UserManagementDialog(const QString& dbConnectionString, QWidget *parent = 0);
    ~UserManagementDialog();

private slots:
    void onCustomContextMenuRequested(const QPoint&);
    void onAddEmployee();
    void onFireEmployee();
    void refreshModel();
    void onChangeLoginpass();

private:
    Ui::UserManagementDialog *ui;
    EmployeeItemModel* employeeModel;
    QSortFilterProxyModel* employeeProxyModel;
};

#endif // USERMANAGEMENTDIALOG_H

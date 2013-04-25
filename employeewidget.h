#ifndef EMPLOYEEWIDGET_H
#define EMPLOYEEWIDGET_H

#include <QWidget>

#include "sqlextension.h"

namespace Ui {
class EmployeeWidget;
}

class EmployeeItemModel;
class QSortFilterProxyModel;

class EmployeeWidget : public QWidget, SqlExtension
{
    Q_OBJECT
    
public:
    explicit EmployeeWidget(const QString& dbConnectionString, QWidget *parent = 0);
    ~EmployeeWidget();
    QVariant getCurrentEmployeeId() const;
    void setEmployeeCurrentId(const int& id);
    void setEmployeeCurrentId();
    QString getCurrentEmployeeName();

Q_SIGNALS:
    void currentIndexChanged(const int& current);

private:
    Ui::EmployeeWidget *ui;
    EmployeeItemModel* employeeModel;
    QSortFilterProxyModel* employeeProxyModel;
};

#endif // EMPLOYEEWIDGET_H

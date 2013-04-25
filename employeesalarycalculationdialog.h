#ifndef EMPLOYEESALARYCALCULATIONDIALOG_H
#define EMPLOYEESALARYCALCULATIONDIALOG_H

#include <QDialog>
#include "sqlextension.h"
#include "jobitemmodel.h"

namespace Ui {
class EmployeeSalaryCalculationDialog;
}

class QStandardItem;
class EmployeeWidget;

class EmployeeSalaryCalculationDialog : public QDialog, SqlExtension
{
    Q_OBJECT
    
public:
    explicit EmployeeSalaryCalculationDialog(const QString& dbConnectionString, QWidget *parent = 0);
    ~EmployeeSalaryCalculationDialog();

private Q_SLOTS:
    void onCurrentEmployeeChanged(const int& current);

private:
    void parseJobs(QHash<QString,QList<Job> >);
    
private:
    Ui::EmployeeSalaryCalculationDialog *ui;    
    JobItemModel* jobs;
    QStandardItem* jobsItem;
    QStandardItem* soldProductsItem;
    EmployeeWidget* employeeWidget;
};

#endif // EMPLOYEESALARYCALCULATIONDIALOG_H

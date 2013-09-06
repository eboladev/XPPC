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
class ItemModelForEmployeePenaltyAndBonuses;

class EmployeeSalaryCalculationDialog : public QDialog, SqlExtension
{
    Q_OBJECT
    
public:
    explicit EmployeeSalaryCalculationDialog(const QString& dbConnectionString, QWidget *parent = 0);
    ~EmployeeSalaryCalculationDialog();

private Q_SLOTS:
    void onSetCurrentEmployeeSalaryPayments(int);
    void onRecalculateSalary(int);
    void onItemChanged(QStandardItem*);
    void onConfirmSalaryClicked();
    void onBonusAndPenaltyManagementClicked();
    void onPABChanged(QString);

    void recalculateJobs(const bool& state);

private:
    void parseJobs();
    void calculateSummarySalary();
    void recalculateSalary();
    void initJobsItem();
    void initSoldProductsItem();
    void initJobsTable();
    void getEmployeePenaltyAndBonuses();

private:
    Ui::EmployeeSalaryCalculationDialog *ui;    
    JobItemModel* jobs;
    QStandardItem* jobsItem;
    QStandardItem* soldProductsItem;
    EmployeeWidget* employeeWidget;
    ItemModelForEmployeePenaltyAndBonuses* penaltyAndBonusesModel;
    QString m_dbConnectionString;
};

#endif // EMPLOYEESALARYCALCULATIONDIALOG_H

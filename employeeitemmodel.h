#ifndef EMPLOYEEITEMMODEL_H
#define EMPLOYEEITEMMODEL_H

#include <QStandardItemModel>

#include "sqlextension.h"

class EmployeeItemModel : public QStandardItemModel, SqlExtension
{
    Q_OBJECT
public:
    explicit EmployeeItemModel(const QString &dbConnectionString, QObject *parent = 0);
    explicit EmployeeItemModel(const QString &dbConnectionString, const bool& isGetOnlyNames, QObject *parent = 0);
    void refreshModel(const bool& isListFired = false, const bool& isGetOnlyNames = false);
    QVariant getCurrentId(const QModelIndex &index);
    QVariant getCurrentId();    
    void appendEmployeeRow(const QVariant&id = QVariant(),
                           const QString& name = trUtf8("ФИО"),
                           const int& rate = 0,
                           const int& percent = 0,
                           const int& sale_percent = 0,
                           const QDateTime& last_salary_date = QDateTime::currentDateTime(),
                           const int& salary_per_day = 0,
                           const QString& login = "",
                           const QString& password = "",
                           const QString& phone = "",
                           const bool& isGetOnlyNames = false);
    /*
employee_id, employee_fio, employee_rate, "
              "employee_percent, employee_sale_percent, "
              "employee_last_salary_date, employee_salaryperday, "
              "login, password, phone
*/
public slots:
    void addEmployee();
    void onFireEmployee(const QModelIndex &index);
    void onItemChanged(QStandardItem*);
};

#endif // EMPLOYEEITEMMODEL_H

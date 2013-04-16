#ifndef EMPLOYEEITEMMODEL_H
#define EMPLOYEEITEMMODEL_H

#include <QStandardItemModel>

#include "sqlextension.h"

enum EmployeeItemRoles
{
    IDrole = Qt::UserRole + 1,
    isPasswordSetRole = Qt::UserRole + 2,
    PhoneRole = Qt::UserRole + 3,
    RateRole = Qt::UserRole + 4,
    PercentRole = Qt::UserRole + 5,
    SalePercentRole = Qt::UserRole + 6,
    SalaryPerDayRole = Qt::UserRole + 7,
    LoginRole = Qt::UserRole + 8,
    NameRole = Qt::DisplayRole,
    GroupIdRole = Qt::UserRole + 9
};

class EmployeeItemModel : public QStandardItemModel, SqlExtension
{
    Q_OBJECT
public:
    explicit EmployeeItemModel(const QString &dbConnectionString, QObject *parent = 0);
    explicit EmployeeItemModel(const QString &dbConnectionString, const bool& isGetOnlyNames, QObject *parent = 0);
    void refreshModel(const bool& isListFired = false, const bool& isGetOnlyNames = false);
    QVariant getCurrentId(const QModelIndex &index);
    QVariant getCurrentId();    
    bool appendEmployeeRow(const bool& isGetOnlyNames = false,
                           const QVariant& id = QVariant(),
                           const QString& name = trUtf8("ФИО"),
                           const int& rate = 0,
                           const int& percent = 0,
                           const int& sale_percent = 0,
                           const QDateTime& last_salary_date = QDateTime::currentDateTime(),
                           const int& salary_per_day = 0,
                           const QString& login = "",
                           const QString& password = "",
                           const QString& phone = "",
                           const QString& groupId = "0");

public Q_SLOTS:
    bool addEmployee(const bool& isGetOnlyNames = false);
    void onFireEmployee(const QModelIndex &index);    
};

#endif // EMPLOYEEITEMMODEL_H

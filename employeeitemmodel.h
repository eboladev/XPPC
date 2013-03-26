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

public slots:
    void addEmployee();
    void onFireEmployee(const QModelIndex &index);
    void onItemChanged(QStandardItem*);
};

#endif // EMPLOYEEITEMMODEL_H

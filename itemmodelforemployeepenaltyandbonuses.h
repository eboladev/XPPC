#ifndef ITEMMODELFOREMPLOYEEPENALTY_H
#define ITEMMODELFOREMPLOYEEPENALTY_H

#include <QStandardItemModel>
#include "sqlextension.h"

enum PABModelRoles
{
    idRole = Qt::UserRole,
    typeRole = Qt::UserRole + 1,
    employeeIdRole = Qt::UserRole + 2
};

class ItemModelForEmployeePenaltyAndBonuses : public QStandardItemModel, SqlExtension
{
    Q_OBJECT
public:
    explicit ItemModelForEmployeePenaltyAndBonuses(const QString& dbConnectionString, QObject *parent = 0);
    void getPenaltyAndBonuses(const QString& id = QString());
    void onAddPAB(const int& amount, const QString& employeeId, const QString& reason);
    void onUpdatePAB(const QModelIndex& index, const int& amount, const QString& reason);
    void onDeletePAB(const QModelIndex&);

    int getAmount(const QModelIndex& index);
    QString getReason(const QModelIndex& index);
    int getEmployeeId(const QModelIndex& index);

private:
    void addPABRow(const QVariant& id, const int& amount, const QString& employee_id,
                   const QString& reason, const QDateTime& date, const QDateTime& inureDate = QDateTime());
    QVariant getId(const QModelIndex& index);
    QString getEmployeeName(const QString& employeeId);
};

#endif // ITEMMODELFOREMPLOYEEPENALTY_H

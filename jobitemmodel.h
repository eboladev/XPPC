#ifndef JOBITEMMODEL_H
#define JOBITEMMODEL_H

#include <QStandardItemModel>

#include "sqlextension.h"

struct Job
{
    QVariant ticketId;
    QVariant id;
    QVariant employeeId;
    QString name;
    int quantity;
    int price;
    bool isHaveGuarantee = false;
    QString guaranteePeriod = "";
};

class JobItemModel : public QStandardItemModel, SqlExtension
{
    Q_OBJECT
public:
    explicit JobItemModel(const QString& dbConnectionString, QObject *parent = 0);
    void getJobs(const QVariant& id);
    void getEmployeeJobs(const QVariant& employeeId);
    void addJob(const QVariant &ticketId,
                const QVariant &employeeId,
                const QString &jobName,
                const int &jobQuantity,
                const int &jobPrice,
                const bool &isHaveGuarantee = false,
                const QString &guaranteePeriod = "");
    void deleteJob(const QModelIndex& index);
    void updateJob(const QModelIndex& index,
                   const QVariant &employeeId,
                   const QString &jobName,
                   const int &jobQuantity,
                   const int &jobPrice,
                   const bool &isHaveGuarantee = false,
                   const QString &guaranteePeriod = "");

    void getCurrentJobName(Job&);

};

#endif // JOBITEMMODEL_H

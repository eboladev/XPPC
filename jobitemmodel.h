#ifndef JOBITEMMODEL_H
#define JOBITEMMODEL_H

#include <QStandardItemModel>

#include "sqlextension.h"

struct Job
{
    QVariant TDCR_ID;
    QVariant id = QVariant();
    QVariant employeeId;    
    QString employeeName;
    QString name;
    int quantity;
    int price;    
    bool isHaveGuarantee = false;
    bool jobPaid = false;
    QString guaranteePeriod = "";
    QDateTime date;
};

class JobItemModel : public QStandardItemModel, SqlExtension
{
    Q_OBJECT
public:
    explicit JobItemModel(const QString& dbConnectionString, QObject *parent = 0);
    int getJobs(const QVariant& id);
    QHash<QString,QList<Job> > getEmployeeJobs(const QVariant& employeeId);
    void addJob(const QVariant &ticketId,
                const QVariant &employeeId,
                const QString &jobName,
                const int &jobQuantity,
                const int &jobPrice,
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
private:
    void addJobRow(const Job& job);
};

#endif // JOBITEMMODEL_H

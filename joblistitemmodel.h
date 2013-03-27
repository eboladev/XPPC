#ifndef JOBLISTITEMMODEL_H
#define JOBLISTITEMMODEL_H

#include <QStandardItemModel>

#include "sqlextension.h"
namespace JobListModelData
{
enum JobListModelData
{
    ID = Qt::UserRole + 1,
    HasGuarantee = Qt::UserRole + 2,
    GuaranteePeriod = Qt::UserRole + 3,
    Text = Qt::DisplayRole
};
}
namespace JobListModelHeader
{
enum JobListModelHeader
{
    Name = 0,
    Price
};
}

class JobListItemModel : public QStandardItemModel, SqlExtension
{
    Q_OBJECT
public:
    explicit JobListItemModel(const QString& dbConnectionName, QObject *parent = 0);
    void refreshModel();
    void addJob(const QString& name,
                const int& price,
                const bool& hasGuarantee,
                const QString& guaranteePeriod);
    bool isJobExist(const QString& name);
private:
    void appendRow(const QVariant& id,
                   const QString& name,
                   const int& price,
                   const bool& hasGuarantee,
                   const QString& guaranteePeriod);
};

#endif // JOBLISTITEMMODEL_H

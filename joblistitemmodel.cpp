#include "joblistitemmodel.h"
#include <QDebug>
JobListItemModel::JobListItemModel(const QString &dbConnectionName, QObject *parent) :
    QStandardItemModel(parent),
    SqlExtension(dbConnectionName)
{
    refreshModel();
}

void JobListItemModel::refreshModel()
{
    clear();
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.exec("select id, name, price, hasguarantee, guarantee_period from job_list");
    while(q.next())
    {
        this->appendRow(q.value(0),
                  q.value(1).toString(),
                  q.value(2).toInt(),
                  q.value(3).toBool(),
                  q.value(4).toString());
    }
}

void JobListItemModel::addJob(const QString &name,
                              const int &price,
                              const bool &hasGuarantee,
                              const QString &guaranteePeriod)
{
    qDebug() << "addJob" << name << price << hasGuarantee << guaranteePeriod;
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.prepare("insert into job_list(name,price,hasguarantee, guarantee_period) "
              "VALUES(?,?,?,?) returning id");
    q.addBindValue(name);
    q.addBindValue(price);
    q.addBindValue(hasGuarantee);
    q.addBindValue(guaranteePeriod);
    if (!q.exec())
        qDebug() << q.lastError() << q.lastQuery();
    if (!q.next())
        return;
    appendRow(q.value(0),name,price,hasGuarantee,guaranteePeriod);
}

void JobListItemModel::appendRow(const QVariant &id,
                                 const QString &name,
                                 const int &price,
                                 const bool &hasGuarantee,
                                 const QString &guaranteePeriod)
{
    qDebug() << "append row" << id << name << price << hasGuarantee << guaranteePeriod;
    QStandardItem* nameItem = new QStandardItem(name);
    nameItem->setData(id, JobListModelData::ID);
    nameItem->setData(hasGuarantee, JobListModelData::HasGuarantee);
    if (hasGuarantee)
        nameItem->setToolTip(trUtf8("Гарантия: нет"));
    else
    {
        nameItem->setToolTip(trUtf8("Срок гарантии: %0").arg(guaranteePeriod));
        nameItem->setData(guaranteePeriod, JobListModelData::GuaranteePeriod);
    }
    QStandardItem* priceItem = new QStandardItem(QString::number(price));
    QStandardItemModel::appendRow(QList<QStandardItem*>() << nameItem << priceItem);
}

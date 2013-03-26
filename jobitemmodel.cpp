#include "jobitemmodel.h"

JobItemModel::JobItemModel(const QString &dbConnectionString, QObject *parent) :
    QStandardItemModel(parent),
    SqlExtension(dbConnectionString)
{
}

void JobItemModel::getJobs(const QVariant &id)
{
    clear();
    setHorizontalHeaderLabels(QStringList() << trUtf8("Мастер") << trUtf8("Наименование")
                                     << trUtf8("Количество") << trUtf8("Цена") << trUtf8("Дата"));
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;

    q.prepare("select employee_FIO,job_name,job_quantity,job_price,Job_date,jot_id from JobOnTicket "
              "join Employee ON (JobOnTicket.Employee_ID=Employee.Employee_ID) where tdc_r_id=?");
    q.addBindValue(id);

    if (!q.exec())
        return;

    while (q.next())
    {
        QStandardItem* fio = new QStandardItem(q.value(0).toString());
        fio->setData(q.value(5));
        QStandardItem* jobName = new QStandardItem(q.value(1).toString());
        QStandardItem* jobQuant = new QStandardItem(q.value(2).toString());
        QStandardItem* jobPrice = new QStandardItem(q.value(3).toString());
        QStandardItem* jobDate = new QStandardItem(q.value(4).toString());
        appendRow(QList<QStandardItem*>() << fio << jobName << jobQuant << jobPrice << jobDate);
    }
}

void JobItemModel::getEmployeeJobs(const QVariant &employeeId)
{
    Q_UNUSED(employeeId);
    qDebug() << Q_FUNC_INFO << "NOT YET IMPLEMENTED";
}

void JobItemModel::addJob(const QVariant& ticketId,
                          const QVariant& employeeId,
                          const QString& jobName,
                          const int& jobQuantity,
                          const int& jobPrice,
                          const bool& isHaveGuarantee,
                          const QString& guaranteePeriod)
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.prepare("insert into JobOnTicket(tdc_r_id,employee_id,job_name,job_quantity,job_price) values(?,?,?,?,?)");
    q.addBindValue(ticketId);
    q.addBindValue(employeeId);
    q.addBindValue(jobName);
    q.addBindValue(jobQuantity);
    q.addBindValue(jobPrice);
    if (!q.exec())
    {
        qDebug() << q.lastError() << q.lastQuery();
        return;
    }
}

void JobItemModel::deleteJob(const QModelIndex &index)
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.prepare("delete from JobOnTicket where jot_id=?");
    q.addBindValue(item(index.row(),0)->data());
    if (!q.exec())
        return;
}

void JobItemModel::updateJob(const QModelIndex &index,
                             const QVariant &employeeId,
                             const QString &jobName,
                             const int &jobQuantity,
                             const int &jobPrice,
                             const bool &isHaveGuarantee,
                             const QString &guaranteePeriod)
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.prepare("update JobOnTicket set employee_id = ?, job_name = ?, "
              "job_quantity = ?, job_price = ? where jot_id = ?");
    q.addBindValue(employeeId);
    q.addBindValue(jobName);
    q.addBindValue(jobQuantity);
    q.addBindValue(jobPrice);
    q.addBindValue(item(index.row(),0)->data());
    q.exec();
}

void JobItemModel::getCurrentJobName(Job &job)
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.prepare("select JobOnTicket.employee_id,job_name,job_quantity,job_price from JobOnTicket "
              "join Employee ON (JobOnTicket.Employee_ID=Employee.Employee_ID) where jot_id=?");
    q.addBindValue(job.id);
    if (!q.exec())
        return;
    if (q.next())
    {
        job.employeeId = q.value(0);
        job.name = q.value(1).toString();
        job.quantity = q.value(2).toInt();
        job.price = q.value(3).toInt();
    }
}

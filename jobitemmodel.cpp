#include "jobitemmodel.h"

#include "usersandpermissionsmanager.h"

JobItemModel::JobItemModel(const QString &dbConnectionString, QObject *parent) :
    QStandardItemModel(parent),
    SqlExtension(dbConnectionString)
{
}

int JobItemModel::getJobs(const QVariant &id)
{
    clear();
    int totalPrice = 0;
    setHorizontalHeaderLabels(QStringList() << trUtf8("Мастер")
                              << trUtf8("Наименование")
                              << trUtf8("Количество")
                              << trUtf8("Цена")
                              << trUtf8("Гарантия")
                              << trUtf8("Дата"));
    QSqlQuery q;
    if (!getSqlQuery(q))
        return -1;

    q.prepare("select employee_FIO,job_name,job_quantity,job_price,Job_date,jot_id,job_guarantee from JobOnTicket "
              "join Employee ON (JobOnTicket.Employee_ID=Employee.Employee_ID) where tdc_r_id = ? order by job_date DESC");
    q.addBindValue(id);

    if (!q.exec())
        return -1;

    while (q.next())
    {
        totalPrice += q.value(3).toInt();
        Job job;
        job.employeeName = q.value(0).toString();
        job.name = q.value(1).toString();
        job.quantity = q.value(2).toInt();
        job.price = q.value(3).toInt();
        job.date = q.value(4).toDateTime();
        job.id = q.value(5);
        job.guaranteePeriod = q.value(6).toString();
        job.TDCR_ID = id;
        addJobRow(job);
    }
    return totalPrice;
}

QHash<QString, QList<Job> > JobItemModel::getEmployeeJobs(const QVariant &employeeId)
{
    QHash<QString,QList<Job> >  result;
    QSqlQuery q;
    if (!getSqlQuery(q))
        return result;

    q.prepare("select job_name,job_quantity,job_price,Job_date,jot_id,tdc_r_id,ticket.ticket_id from JobOnTicket "
              "join Employee ON (JobOnTicket.Employee_ID=Employee.Employee_ID) "
              "join ticket ON (jobonticket.tdc_r_id = ticket.id) "
              "where Employee.Employee_ID = ? and job_payed_datetime IS NULL");
    q.addBindValue(employeeId);

    if (!q.exec())
        return result;
    while (q.next())
    {
        Job job;
        job.name = q.value(0).toString();
        job.quantity = q.value(1).toInt();
        job.price = q.value(2).toInt();
        job.date = q.value(3).toDateTime();
        job.id = q.value(4);
        job.TDCR_ID = q.value(5);
        result[q.value(6).toString()].append(job);
    }
    return result;
}

void JobItemModel::addJob(const QVariant& ticketId,
                          const QVariant& employeeId,
                          const QString& jobName,
                          const int& jobQuantity,
                          const int& jobPrice,
                          const QString& guaranteePeriod)
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.prepare("insert into JobOnTicket(tdc_r_id,employee_id,job_name,job_quantity,job_price,job_guarantee) values(?,?,?,?,?,?)");
    q.addBindValue(ticketId);
    q.addBindValue(employeeId);
    q.addBindValue(jobName);
    q.addBindValue(jobQuantity);
    q.addBindValue(jobPrice);
    q.addBindValue(guaranteePeriod);
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

void JobItemModel::addJobRow(const Job &job)
{
    QStandardItem* fio = new QStandardItem(job.employeeName);
    fio->setData(job.id);
    fio->setData(job.TDCR_ID,Qt::UserRole + 2);
    QStandardItem* jobName = new QStandardItem(job.name);
    QStandardItem* jobQuant = new QStandardItem(QString::number(job.quantity));
    QStandardItem* jobPrice = new QStandardItem(QString::number(job.price));
    QStandardItem* jobDate = new QStandardItem(job.date.toString("dd-MM-yyyy hh:mm:ss"));
    QStandardItem* jobGuarantee = new QStandardItem(job.guaranteePeriod);
    appendRow(QList<QStandardItem*>() << fio << jobName << jobQuant << jobPrice << jobGuarantee << jobDate);
}

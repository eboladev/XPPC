#include "itemmodelforemployeepenaltyandbonuses.h"

#include <QDebug>

ItemModelForEmployeePenaltyAndBonuses::ItemModelForEmployeePenaltyAndBonuses(const QString &dbConnectionString, QObject *parent) :
    QStandardItemModel(parent),
    SqlExtension(dbConnectionString)
{
}

void ItemModelForEmployeePenaltyAndBonuses::getPenaltyAndBonuses(const QString &id)
{
    clear();
    setHorizontalHeaderLabels(QStringList()
                              << trUtf8("ФИО")
                              << trUtf8("Сумма")
                              << trUtf8("Причина")
                              << trUtf8("Дата")
                              << trUtf8("Вступило в силу"));

    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    QString PABquery = QString("select id, amount, employee_id, reason, date, inure_date from "
                               "employee_penalty_and_bonuses %0").arg(!id.isEmpty() ? "where employee_id = ?" : QString());
    q.prepare(PABquery);
    if (!id.isEmpty())
        q.addBindValue(id);

    if (!q.exec())
        qDebug() << q.lastError() << q.lastQuery();

    while (q.next())
    {
        addPABRow(q.value(0),
                  q.value(1).toInt(),
                  q.value(2).toString(),
                  q.value(3).toString(),
                  q.value(4).toDateTime(),
                  q.value(5).toDateTime());
    }
}

void ItemModelForEmployeePenaltyAndBonuses::onAddPAB(const int &amount, const QString &employeeId, const QString &reason)
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;

    q.prepare("insert into employee_penalty_and_bonuses(amount, employee_id, reason) VALUES(?,?,?) returning id,date");
    q.addBindValue(amount);
    q.addBindValue(employeeId);
    q.addBindValue(reason);
    q.exec();
    if (q.next())
        addPABRow(q.value(0),
                  amount,
                  employeeId,
                  reason,
                  q.value(1).toDateTime());
}

void ItemModelForEmployeePenaltyAndBonuses::onDeletePAB(const QModelIndex &index)
{
    if (!index.isValid())
        return;
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.prepare("delete from employee_penalty_and_bonuses where id = ?");
    q.addBindValue(getId(index));
    q.exec();
    removeRow(index.row());
}

int ItemModelForEmployeePenaltyAndBonuses::getAmount(const QModelIndex &index)
{
    return item(index.row(),1)->text().toInt();
}

QString ItemModelForEmployeePenaltyAndBonuses::getReason(const QModelIndex &index)
{
    return item(index.row(),2)->text();
}

int ItemModelForEmployeePenaltyAndBonuses::getEmployeeId(const QModelIndex &index)
{
    return item(index.row(),0)->data(employeeIdRole).toInt();
}

void ItemModelForEmployeePenaltyAndBonuses::onUpdatePAB(const QModelIndex &index, const int &amount, const QString &reason)
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.prepare("update employee_penalty_and_bonuses set amount = ?, reason = ? where id = ?");
    q.addBindValue(amount);
    q.addBindValue(reason);
    q.addBindValue(getId(index));
    if (!q.exec())
    {
        qDebug() << q.lastError() << q.lastQuery();
    }
}

void ItemModelForEmployeePenaltyAndBonuses::addPABRow(const QVariant &id, const int &amount,
                                                      const QString &employee_id, const QString &reason,
                                                      const QDateTime &date, const QDateTime &inureDate)
{
    QStandardItem* employeeNameItem = new QStandardItem(getEmployeeName(employee_id));
    employeeNameItem->setData(id,idRole);
    employeeNameItem->setData(employee_id,employeeIdRole);
    employeeNameItem->setEditable(false);
    QStandardItem* amountItem = new QStandardItem(QString::number(amount));
    QStandardItem* reasonItem = new QStandardItem(reason);
    reasonItem->setToolTip(reason);
    QStandardItem* dateItem = new QStandardItem(date.toString("dd-MM-yyyy hh:mm:ss"));
    dateItem->setEditable(false);
    QStandardItem* inureDateItem = new QStandardItem(inureDate.isNull() ? inureDate.toString("dd-MM-yyyy hh:mm:ss") : trUtf8("Нет"));
    inureDateItem->setEditable(false);
    appendRow(QList<QStandardItem*>() << employeeNameItem << amountItem << reasonItem << dateItem << inureDateItem);
}

QVariant ItemModelForEmployeePenaltyAndBonuses::getId(const QModelIndex &index)
{
    return item(index.row(),0)->data(idRole);
}

QString ItemModelForEmployeePenaltyAndBonuses::getEmployeeName(const QString &employeeId)
{
    QString employeeName = QString();

    QSqlQuery q;
    if (!getSqlQuery(q))
        return QString();
    q.prepare("select employee_fio from employee where employee_id = ?");
    q.addBindValue(employeeId);
    q.exec();
    if (q.next())
        employeeName = q.value(0).toString();

    return employeeName;
}

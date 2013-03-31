#include "employeeitemmodel.h"
#include "setupmanager.h"

EmployeeItemModel::EmployeeItemModel(const QString &dbConnectionString, QObject *parent) :
    QStandardItemModel(parent),
    SqlExtension(dbConnectionString)
{
    refreshModel();
}

EmployeeItemModel::EmployeeItemModel(const QString &dbConnectionString, const bool &isGetOnlyNames, QObject *parent) :
    QStandardItemModel(parent),
    SqlExtension(dbConnectionString)
{
    refreshModel(false, isGetOnlyNames);
}

void EmployeeItemModel::refreshModel(const bool &isListFired, const bool &isGetOnlyNames)
{
    clear();

    setHorizontalHeaderLabels(QStringList() << trUtf8("ФИО") << trUtf8("Телефон") << trUtf8("Оплата за ремонт") <<
                                      trUtf8("Процент") << trUtf8("Процент с продаж") <<
                                      trUtf8("Дата з\\п") << trUtf8("З\\п в день") <<
                                      trUtf8("Логин") << trUtf8("Пароль"));

    QSqlQuery q;

    if (!getSqlQuery(q))
        return;

    q.prepare("select employee_id, employee_fio, employee_rate, "
              "employee_percent, employee_sale_percent, "
              "employee_last_salary_date, employee_salaryperday, "
              "login, password, phone from employee where fired = ? ORDER BY employee_id ASC");
    q.addBindValue(isListFired);
    q.exec();

    while (q.next())
    {
        QStandardItem* fio = new QStandardItem(q.value(1).toString());
        fio->setData(q.value(0));
        fio->setToolTip(fio->text());
        if (!isGetOnlyNames)
        {
            QStandardItem* rate = new QStandardItem(q.value(2).toString());
            rate->setToolTip(trUtf8("Оплата за ремонт по квитанции."));
            QStandardItem* percent = new QStandardItem(q.value(3).toString());
            percent->setToolTip(trUtf8("Процент с ремонта."));
            QStandardItem* sPercent = new QStandardItem(q.value(4).toString());
            sPercent->setToolTip(trUtf8("Процент с продаж."));
            QStandardItem* sDate = new QStandardItem(q.value(5).toString());
            sDate->setToolTip(trUtf8("Дата последнего получения з\\п"));
            QStandardItem* spDay = new QStandardItem(q.value(6).toString());
            spDay->setToolTip(trUtf8("Оплата за раб. день"));
            QStandardItem* login = new QStandardItem(q.value(7).toString());
            login->setToolTip(login->text());
            QStandardItem* pass = new QStandardItem(q.value(8).toString().isEmpty() ? trUtf8("Не задан") : trUtf8("Задан"));
            pass->setData(q.value(8).toString().isEmpty() ? false : true);
            QStandardItem* phone = new QStandardItem(q.value(9).toString());
            phone->setToolTip(phone->text());
            appendRow(QList<QStandardItem*>() << fio << phone << rate
                             << percent << sPercent << sDate << spDay << login << pass);
        }
        else
            appendRow(fio);
    }
}

void EmployeeItemModel::addEmployee()
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.prepare("insert into employee(employee_fio) VALUES(?)");
    q.addBindValue(trUtf8("ФИО"));
    if (!q.exec())
        qDebug() << q.lastError() << q.lastQuery();   
}

void EmployeeItemModel::onFireEmployee(const QModelIndex &index)
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.prepare("update employee set fired = ? where employee_id = ?");
    q.addBindValue(true);
    q.addBindValue(getCurrentId(index));
    if (!q.exec())
        qDebug() << q.lastError() << q.lastQuery();
}

void EmployeeItemModel::onItemChanged(QStandardItem *itemChanged)
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    QModelIndex idx = itemChanged->index();
    if (idx.column() == 3 || idx.column() == 4 || idx.column() == 5 || idx.column() == 6 || idx.column() == 7)
        setData(idx,itemChanged->text().toInt(),Qt::DisplayRole);
    q.prepare("update employee set employee_fio = ?, employee_rate = ?, "
              "employee_percent = ?, employee_sale_percent = ?, "
              "employee_last_salary_date = ?, employee_salaryperday = ?, "
              "phone = ? "
              "where employee_id = ?");

    q.addBindValue(item(idx.row(),0)->text());
    q.addBindValue(item(idx.row(),2)->text().toInt());
    q.addBindValue(item(idx.row(),3)->text().toInt());
    q.addBindValue(item(idx.row(),4)->text().toInt());
    q.addBindValue(item(idx.row(),5)->text());
    q.addBindValue(item(idx.row(),6)->text().toInt());
    q.addBindValue(item(idx.row(),1)->text());
    q.addBindValue(item(idx.row(),0)->data());
    if (!q.exec())
        qDebug() << q.lastError() << q.lastQuery();
}

QVariant EmployeeItemModel::getCurrentId(const QModelIndex &index)
{
    return item(index.row(),0)->data();
}

QVariant EmployeeItemModel::getCurrentId()
{
    return SetupManager::instance()->getCurrentUserId();
}

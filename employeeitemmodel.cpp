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

    if (!isGetOnlyNames)
        setHorizontalHeaderLabels(QStringList() << trUtf8("ФИО") << trUtf8("Телефон") << trUtf8("Оплата за ремонт") <<
                                  trUtf8("Процент") << trUtf8("Процент с продаж") <<
                                  trUtf8("Дата з\\п") << trUtf8("З\\п в день") <<
                                  trUtf8("Логин") << trUtf8("Пароль"));
    else
        setHorizontalHeaderLabels(QStringList() << trUtf8("ФИО"));

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
        appendEmployeeRow(isGetOnlyNames,
                          q.value(0),
                          q.value(1).toString(),
                          q.value(2).toInt(),
                          q.value(3).toInt(),
                          q.value(4).toInt(),
                          q.value(5).toDateTime(),
                          q.value(6).toInt(),
                          q.value(7).toString(),
                          q.value(8).toString(),
                          q.value(9).toString()
                          );
    }
}

bool EmployeeItemModel::addEmployee(const bool &isGetOnlyNames)
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return false;
    q.prepare("insert into employee(employee_fio) VALUES(?) returning employee_id");
    q.addBindValue(trUtf8("ФИО"));
    if (!q.exec())
    {
        qDebug() << q.lastError() << q.lastQuery();
        return false;
    }
    if (!q.next())
        return false;

    return appendEmployeeRow(isGetOnlyNames,q.value(0));
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

QVariant EmployeeItemModel::getCurrentId(const QModelIndex &index)
{
    return item(index.row(),0)->data();
}

QVariant EmployeeItemModel::getCurrentId()
{
    return SetupManager::instance()->getCurrentUserId();
}

bool EmployeeItemModel::appendEmployeeRow(const bool &isGetOnlyNames,
                                          const QVariant &id,
                                          const QString &name,
                                          const int &rate,
                                          const int &percent,
                                          const int &sale_percent,
                                          const QDateTime &last_salary_date,
                                          const int &salary_per_day,
                                          const QString &login,
                                          const QString &password,
                                          const QString &phone
                                          )
{
    QStandardItem* fio = new QStandardItem(name);
    fio->setData(id, IDrole);
    fio->setToolTip(fio->text());
    if (!isGetOnlyNames)
    {
        QStandardItem* rateItem = new QStandardItem(QString::number(rate));
        rateItem->setToolTip(trUtf8("Оплата за ремонт по квитанции."));
        QStandardItem* percentItem = new QStandardItem(QString::number(percent));
        percentItem->setToolTip(trUtf8("Процент с ремонта."));
        QStandardItem* sPercent = new QStandardItem(QString::number(sale_percent));
        sPercent->setToolTip(trUtf8("Процент с продаж."));
        QStandardItem* sDate = new QStandardItem(last_salary_date.toString("dd-MM-yyyy_hh-mm-ss"));
        sDate->setToolTip(trUtf8("Дата последнего получения з\\п"));
        QStandardItem* spDay = new QStandardItem(QString::number(salary_per_day));
        spDay->setToolTip(trUtf8("Оплата за раб. день"));
        QStandardItem* loginItem = new QStandardItem(login);
        loginItem->setToolTip(loginItem->text());
        QStandardItem* pass = new QStandardItem(password.isEmpty() ? trUtf8("Не задан") : trUtf8("Задан"));
        pass->setData(password.isEmpty() ? false : true);
        QStandardItem* phoneItem = new QStandardItem(phone);
        phoneItem->setToolTip(phoneItem->text());
        appendRow(QList<QStandardItem*>() << fio << phoneItem << rateItem
                         << percentItem << sPercent << sDate << spDay << loginItem << pass);
        return true;
    }
    else
    {
        fio->setData(rate,RateRole);
        fio->setData(percent, PercentRole);
        fio->setData(sale_percent, SalePercentRole);
        fio->setData(salary_per_day, SalaryPerDayRole);
        fio->setData(login, LoginRole);
        fio->setData(phone, PhoneRole);
        fio->setData(password.isEmpty() ? false : true, isPasswordSetRole);
        appendRow(fio);        
        return true;
    }
}

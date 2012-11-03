#ifndef SQLEXTENSION_H
#define SQLEXTENSION_H

#include <QString>
#include <QtSql>

// класс добавляет поддержку sql запросов в родиетльский класс
class SqlExtension
{
public:
    explicit SqlExtension(const QString &connectionString);
    explicit SqlExtension();
    bool getSqlQuery(QSqlQuery &q);
    QString getDbConnectionString() const;
    void setDbConnectionString(const QString &dbConnectionString);

    QSqlDatabase getSqlDatabase();

private:
    QString dbConnectionString;
};

#endif // SQLEXTENSION_H

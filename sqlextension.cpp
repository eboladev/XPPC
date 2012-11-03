#include "sqlextension.h"
#include <QString>

SqlExtension::SqlExtension(const QString &connectionString) :
    dbConnectionString(connectionString)
{

}

SqlExtension::SqlExtension() :
    dbConnectionString(QString())
{
}

QString SqlExtension::getDbConnectionString() const
{
    return dbConnectionString;
}

void SqlExtension::setDbConnectionString(const QString &dbConnectionString)
{
    this->dbConnectionString = dbConnectionString;
}

QSqlDatabase SqlExtension::getSqlDatabase()
{
    return QSqlDatabase::database(dbConnectionString, false);
}

bool SqlExtension::getSqlQuery(QSqlQuery &q)
{
    QSqlDatabase db = QSqlDatabase::database(dbConnectionString, false);
    q = QSqlQuery(db);
    return db.isOpen();
}

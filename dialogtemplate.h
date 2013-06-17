#ifndef DIALOGTEMPLATE_H
#define DIALOGTEMPLATE_H

#include "setupmanager.h"

#include <QWidget>
#include <QUuid>

class DialogTemplate
{
public:
    DialogTemplate() {}
    template<class T>
    static bool executeDialogWithDbCheck(QWidget *parent = 0)
    {
        bool ok = false;
        QString dbConnectionString = QUuid::createUuid().toString().replace("}","").replace("-", "").right(16).prepend("a");
        {
            if (setupManager->openSQLDatabase(dbConnectionString) != SetupManager::FBCorrect)
            {
                QSqlDatabase::removeDatabase(dbConnectionString);
                return ok;
            }

            QSqlDatabase db = QSqlDatabase::database(dbConnectionString, false);
            if (!db.isOpen())
            {
                QSqlDatabase::removeDatabase(dbConnectionString);
                return ok;
            }

            db.transaction();
            T ap(dbConnectionString,parent);
            ap.setWindowModality(Qt::WindowModal);
            if (ap.exec())
            {
                ok = true;
                db.commit();
            }
            else
            {
                ok = false;
                db.rollback();
            }

            db.close();

        }
        QSqlDatabase::removeDatabase(dbConnectionString);
        return ok;
    }
    template<class T>
    static bool executeDialogWithDbCheck(int par1,QWidget *parent = 0)
    {
        bool ok = false;
        QString dbConnectionString = QUuid::createUuid().toString().replace("}","").replace("-", "").right(16).prepend("a");
        {
            if (setupManager->openSQLDatabase(dbConnectionString) != SetupManager::FBCorrect)
            {
                QSqlDatabase::removeDatabase(dbConnectionString);
                return ok;
            }

            QSqlDatabase db = QSqlDatabase::database(dbConnectionString, false);
            if (!db.isOpen())
            {
                QSqlDatabase::removeDatabase(dbConnectionString);
                return ok;
            }

            db.transaction();
            T ap(dbConnectionString,par1,parent);
            ap.setWindowModality(Qt::WindowModal);
            if (ap.exec())
            {
                ok = true;
                db.commit();
            }
            else
            {
                ok = false;
                db.rollback();
            }

            db.close();

        }
        QSqlDatabase::removeDatabase(dbConnectionString);
        return ok;
    }
    template<class T>
    static bool executeDialogWithDbCheck(int par1,int par2,QWidget *parent = 0)
    {
        bool ok = false;
        QString dbConnectionString = QUuid::createUuid().toString().replace("}","").replace("-", "").right(16).prepend("a");
        {
            if (setupManager->openSQLDatabase(dbConnectionString) != SetupManager::FBCorrect)
            {
                QSqlDatabase::removeDatabase(dbConnectionString);
                return ok;
            }

            QSqlDatabase db = QSqlDatabase::database(dbConnectionString, false);
            if (!db.isOpen())
            {
                QSqlDatabase::removeDatabase(dbConnectionString);
                return ok;
            }

            db.transaction();
            T ap(dbConnectionString,par1,par2,parent);
            ap.setWindowModality(Qt::WindowModal);
            if (ap.exec())
            {
                ok = true;
                db.commit();
            }
            else
            {
                ok = false;
                db.rollback();
            }

            db.close();

        }
        QSqlDatabase::removeDatabase(dbConnectionString);
        return ok;
    }
    template<class T>//probably won't work
    static bool executeDialogWithDbCheck2(const QString &dbConnectionString, QSqlQuery &queryForSavepoint, QWidget *parent = 0)
    {
        bool ok = false;
        QString savePointName = QString("C") + QUuid::createUuid().toString().remove("{").remove("}").remove("-").right(16);
        bool flag = queryForSavepoint.exec(QString("savepoint %0").arg(savePointName));
        Q_ASSERT_X(flag, Q_FUNC_INFO, "can't save current db state");
        T ap(dbConnectionString,parent);
        ap.setWindowModality(Qt::WindowModal);
        if (ap.exec())
        {
            ok = true;
            flag = queryForSavepoint.exec(QString("release savepoint %0").arg(savePointName));
            Q_ASSERT_X(flag, Q_FUNC_INFO, "can't release to savepoint");
        }
        else
        {
            ok = false;
            flag = queryForSavepoint.exec(QString("rollback to savepoint %0").arg(savePointName));
            Q_ASSERT_X(flag, Q_FUNC_INFO, "can't rollback to savepoint");
        }
        return ok;
    }
};

#endif // DIALOGTEMPLATE_H

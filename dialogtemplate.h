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
};

#endif // DIALOGTEMPLATE_H

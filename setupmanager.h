#ifndef SETUPMANAGER_H
#define SETUPMANAGER_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDriver>

#define setupManager (static_cast<SetupManager *>(SetupManager::instance()))

class SetupManager : public QObject
{
    Q_OBJECT

public:
    enum FireBirdError
    {
        FBCorrect,
        FBCantOpen,
        FBWrongVersion,
        FBAlreadyOpened
    };

public:

    explicit SetupManager(QObject *parent = 0);
    ~SetupManager();
    static SetupManager* instance();

    void setCurrentBranch(const QVariant &id);
    int getCurrentBranch() const;

    void setClientNameCompleterEnabled(const bool &enabled);
    bool getClientNameCompleterEnabled();

    QSqlDatabase getDatabase(const QString &connectionName = "XP");
    QString getDBDefaultConnectionName() const {return QString("XP");}
    bool getSqlQueryForDB(QSqlQuery &q, const QString &connectionName = "XP");

    int openSQLDatabase(QString connectionName = "XP");

    static QString getFBFileLocation();
    static void setFBFileLocation(QString location);

    static QString getDbHostName();
    static QString getDbPassword();
    static QString getDbName();
    static QString getDbUserName();
    static int getDbPort();

    static void setDbHostName(QString host);
    static void setDbPassword (QString password);
    static void setDbName(QString name);
    static void setDbUserName(QString name);
    static void setDbPort(QString port);

public:
    static void encryptDecrypt(QByteArray &ba);

private:    
    int currentBranch;
    int fbStatus;
    int dbStatus;
};

#endif // SETUPMANAGER_H

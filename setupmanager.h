#ifndef SETUPMANAGER_H
#define SETUPMANAGER_H

#include <QObject>
#include <QSettings>
#include <QDir>
#include <QDateTime>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDriver>

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
    int getPermissions();
    QString getCurrentUser();
    QString getDisplayName();
    void setPermissons(int permisson);
    void setCurrentUser(QString user);
    void setDisplayName(QString name);
    int getDbSQLStatus() {return dbStatus;}

    QSqlDatabase getDatabase(const QString &connectionName = "XP");
    QString getDBDefaultConnectionName() const {return QString("XP");}
    bool getSqlQueryForDB(QSqlQuery &q, const QString &connectionName = "XP");

    int openSQLDatabase(QString connectionName = "XP");

    static QString getFBFileLocation();
    static void setFBFileLocation(QString location);

    //локальная база
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
signals:

public slots:

private:
    QString currentUser;
    QString displayName;
    int permission;
    int fbStatus;
    int dbStatus;
};

#endif // SETUPMANAGER_H

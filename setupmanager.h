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

    //    enum SetupPermission
    //    {
    //        SPSetup = 0x01,
    //        SPUsk = 0x02,
    //        SPWebCam = 0x04,
    //        SPGsmModem = 0x08,
    //        SPSendNet = 0x10
    //    };

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

    //    static QString getSettingFile();

    //    QSqlDatabase getSqliteDatabase();
    QSqlDatabase getDatabase(const QString &connectionName = "XP");
    //    QSqlDatabase getFireBirdSQLDatabase();
    QString getDBDefaultConnectionName() const {return QString("XP");}
    bool getSqlQueryForDB(QSqlQuery &q, const QString &connectionName = "XP");

    //    bool openSqliteDatabase();
    //    int openFireBirdSQLDatabase();
    int openSQLDatabase(QString connectionName = "XP");

    //локальнкая база sqlite (к удалению)
    // TODO: remove sqlite database from project
    //    static QString getSqliteFileLocation();

    //    static void setSqliteFileLocation(QString location);

    //удаленная база firebirdsql
    //    static QString getFBConnectionName(){return tr("fb");}
    //    static QString getFBHostName();
    //    static QString getFBPassword();
    //    static QString getFBFileLocation();
    //    static QString getFBUserName();

    //    static void setFBHostName(QString host);
    //    static void setFBPassword (QString password);
    //    static void setFBFileLocation(QString location);
    //    static void setFBUserName(QString name);

    //локальная база firebirdsql
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

    //    bool sqliteStatus;
    //    int fbStatus;
    int dbStatus;
};

#endif // SETUPMANAGER_H

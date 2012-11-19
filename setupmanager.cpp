#include "setupmanager.h"


SetupManager::SetupManager(QObject *parent) :
    QObject(parent), dbStatus(FBCantOpen)
{
}

SetupManager::~SetupManager()
{
}

Q_GLOBAL_STATIC(SetupManager, singlton_instance)
SetupManager * SetupManager::instance()
{
    return singlton_instance();
}

int SetupManager::getPermissions()
{
    return permission;
}

QString SetupManager::getCurrentUser()
{
    return currentUser;
}

void SetupManager::setPermissons(int p)
{
    permission = p;
}

void SetupManager::setCurrentUser(QString user)
{
    currentUser = user;
}

QString SetupManager::getDisplayName()
{
    return displayName;
}

void SetupManager::setDisplayName(QString name)
{
    displayName = name;
}

QString SetupManager::getFBFileLocation()
{
    QSettings settings; //(getSettingFile(),  QSettings::IniFormat);
    if (settings.contains("db/DatabaseName")){
        return settings.value("db/DatabaseName").toString();
    } else {
        //QString fbfilelocation = QDir::currentPath().append("/HP.FDB"); //fb
        QString fbfilelocation = "hpnew";
        settings.setValue("db/DatabaseName", fbfilelocation);
        return fbfilelocation;
    }
}

void SetupManager::setFBFileLocation(QString location)
{
    QSettings settings; //(getSettingFile(),  QSettings::IniFormat);
    // settings.setValue("db/fbfilelocation", location);
    settings.setValue("db/DatabaseName", location);
}

QString SetupManager::getDbHostName()
{
    QSettings settings;//(getSettingFile(),  QSettings::IniFormat);
    if (settings.contains("db/HostName")){
        return settings.value("db/HostName").toString();
    } else {
        QString fbhostname = "195.46.162.200";
        //QString fbhostname = "localhost"; //fb
        settings.setValue("db/HostName", fbhostname);
        return fbhostname;
    }
}

QString SetupManager::getDbPassword()
{
    QSettings settings; //(getSettingFile(),  QSettings::IniFormat);
    if (settings.contains("db/Password")){
        QByteArray ba = settings.value("db/Password").toByteArray();
        encryptDecrypt(ba);
        return QString::fromUtf8(ba.data(), ba.count());
    }
}

QString SetupManager::getDbName()
{
    QSettings settings; //(getSettingFile(),  QSettings::IniFormat);
    if (settings.contains("db/DatabaseName")){
        return settings.value("db/DatabaseName").toString();
    } else {
        QString fbfilelocation = "Service_new";
        //QString fbfilelocation = QDir::currentPath().append("/db/HP.FDB"); //fb
        settings.setValue("db/DatabaseName", fbfilelocation);
        return fbfilelocation;
    }
}

QString SetupManager::getDbUserName()
{
    QSettings settings; //(getSettingFile(),  QSettings::IniFormat);
    if (settings.contains("db/UserName")){
        return settings.value("db/UserName").toString();
    } else {
        //QString fbusername = "SYSDBA"; //fb
        QString fbusername = "";
        settings.setValue("db/UserName", fbusername);
        return fbusername;
    }
}

int SetupManager::getDbPort()
{
    QSettings settings; //(getSettingFile(),  QSettings::IniFormat);
    if (settings.contains("db/Port")){
        return settings.value("db/Port").toInt();
    } else {
        QString fbusername = "3306";
        settings.setValue("db/Port", fbusername);
        return fbusername.toInt();
    }
}

void SetupManager::setDbPort(QString port)
{
    QSettings settings; //(getSettingFile(),  QSettings::IniFormat);
    settings.setValue("db/Port", port);
}

void SetupManager::setDbHostName(QString host)
{
    QSettings settings; //(getSettingFile(),  QSettings::IniFormat);
    settings.setValue("db/HostName", host);
}

void SetupManager::setDbPassword(QString password)
{
    QSettings settings; //(getSettingFile(),  QSettings::IniFormat);
    QByteArray ba = password.toUtf8();
    encryptDecrypt(ba);
    settings.setValue("db/Password", ba);
}

void SetupManager::setDbName(QString name)
{
    QSettings settings; //(getSettingFile(),  QSettings::IniFormat);
    settings.setValue("db/DatabaseName", name);
}

void SetupManager::setDbUserName(QString name)
{
    QSettings settings; //(getSettingFile(),  QSettings::IniFormat);
    settings.setValue("db/UserName", name);
}

void SetupManager::encryptDecrypt(QByteArray &ba)
{
    char c = 55;
    for (int i=0; i<ba.count();++i){
        ba[i] = ba.at(i) ^ c;
        c += 34;
    }
}

int SetupManager::openSQLDatabase(QString connectionName)
{   /* fb */
     QSqlDatabase fireBirdSQLDatabase =  QSqlDatabase::database(connectionName.toLatin1(), false);
    if (fireBirdSQLDatabase.isOpen())
        return FBAlreadyOpened;

    dbStatus = FBCorrect;

    fireBirdSQLDatabase =  QSqlDatabase::database(connectionName.toLatin1(), false);
    if (fireBirdSQLDatabase.isOpen()) return FBCorrect;
    fireBirdSQLDatabase = QSqlDatabase::addDatabase("QFIREBIRD", connectionName.toLatin1());
    fireBirdSQLDatabase.setDatabaseName(getDbName()); //getFBFileLocation()
    fireBirdSQLDatabase.setHostName(getDbHostName());
    fireBirdSQLDatabase.setUserName(getDbUserName());
    fireBirdSQLDatabase.setPassword(getDbPassword());
    fireBirdSQLDatabase.setConnectOptions("CHARSET=UTF-8");
    if  (!fireBirdSQLDatabase.open()) {
        dbStatus =  FBCantOpen;
        return dbStatus;
    }

    if (dbStatus != FBCorrect) fireBirdSQLDatabase.close();

    return dbStatus;
    /* MYSQL */
    /*QSqlDatabase mySQLDatabase =  QSqlDatabase::database(connectionName.toLatin1(), false);
    if (mySQLDatabase.isOpen())
        return FBAlreadyOpened;

    dbStatus = FBCorrect;
    mySQLDatabase = QSqlDatabase::addDatabase("QMYSQL", connectionName.toLatin1());
    mySQLDatabase.setDatabaseName(getDbName());
    mySQLDatabase.setHostName(getDbHostName());
    mySQLDatabase.setPort(getDbPort());
    mySQLDatabase.setUserName(getDbUserName());
    mySQLDatabase.setPassword(getDbPassword());

    if  (!mySQLDatabase.open()) {
        dbStatus =  FBCantOpen;
        return dbStatus;
    }

    if (dbStatus != FBCorrect) mySQLDatabase.close();
    return dbStatus;*/
}

QSqlDatabase SetupManager::getDatabase(const QString &connectionName)
{    
    return QSqlDatabase::database(connectionName);
}

bool SetupManager::getSqlQueryForDB(QSqlQuery &q, const QString &connectionName)
{
    QSqlDatabase db = QSqlDatabase::database(connectionName);
    if (!db.isOpen())
        return false;
    q = QSqlQuery(db);
    return true;
}

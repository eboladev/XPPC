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

//QString SetupManager::getSettingFile()
//{
//#ifdef Q_OS_UNIX
//    return QDir::homePath() + "/.integra/settings.ini";
//#else
//    return "settings.ini";
//#endif
//}

//QString SetupManager::getFBHostName()
//{
//    QSettings settings; //(getSettingFile(),  QSettings::IniFormat);
//    if (settings.contains("db/fbhostname")){
//        return settings.value("db/fbhostname").toString();
//    } else {
//        QString fbhostname = "localhost";
//        settings.setValue("db/fbhostname", fbhostname);
//        return fbhostname;
//    }
//}

//QString SetupManager::getFBPassword()
//{
//    QSettings settings; //(getSettingFile(),  QSettings::IniFormat);
//    if (settings.contains("db/fbpassword")){
//        QByteArray ba = settings.value("db/fbpassword").toByteArray();
//        encryptDecrypt(ba);
//        return QString::fromUtf8(ba.data(), ba.count());
//    } else {
//        QString fbpassword = "masterkey";
//        QByteArray ba = fbpassword.toUtf8();
//        encryptDecrypt(ba);
//        settings.setValue("db/fbpassword", ba);
//        return fbpassword;
//    }
//}

QString SetupManager::getFBFileLocation()
{
    QSettings settings; //(getSettingFile(),  QSettings::IniFormat);
    if (settings.contains("db/DatabaseName")){
        return settings.value("db/DatabaseName").toString();
    } else {
        QString fbfilelocation = QDir::currentPath().append("/HP.FDB");
        settings.setValue("db/DatabaseName", fbfilelocation);
        return fbfilelocation;
    }
}

//void SetupManager::setFBHostName(QString host)
//{
//    QSettings settings; //(getSettingFile(),  QSettings::IniFormat);
//    settings.setValue("db/fbhostname", host);
//}

//void SetupManager::setFBPassword(QString password)
//{
//    QSettings settings; //(getSettingFile(),  QSettings::IniFormat);
//    QByteArray ba = password.toUtf8();
//    encryptDecrypt(ba);
//    settings.setValue("db/fbpassword", ba);
//}

void SetupManager::setFBFileLocation(QString location)
{
    QSettings settings; //(getSettingFile(),  QSettings::IniFormat);
   // settings.setValue("db/fbfilelocation", location);
     settings.setValue("db/DatabaseName", location);
}

//QString SetupManager::getSqliteFileLocation()
//{
//    QSettings settings; //(getSettingFile(),  QSettings::IniFormat);
//    if (settings.contains("db/sqlitefilelocation")){
//        return settings.value("db/sqlitefilelocation").toString();
//    } else {
//        QString sqliteFileLocation = QDir::currentPath().append("/HP.FDB");
//        settings.setValue("db/sqlitefilelocation", sqliteFileLocation);
//        return sqliteFileLocation;
//    }
//}

QString SetupManager::getDbHostName()
{
    QSettings settings;//(getSettingFile(),  QSettings::IniFormat);
    if (settings.contains("db/HostName")){
        return settings.value("db/HostName").toString();
    } else {
        //QString fbhostname = "195.46.162.200";
        QString fbhostname = "localhost";
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
    } else {
        //QString fbpassword = "dctvgbpltw";
        QString fbpassword = "masterkey";
        QByteArray ba = fbpassword.toUtf8();
        encryptDecrypt(ba);
        settings.setValue("db/Password", ba);
        return fbpassword;
    }
}

QString SetupManager::getDbName()
{
    QSettings settings; //(getSettingFile(),  QSettings::IniFormat);
    if (settings.contains("db/DatabaseName")){
        return settings.value("db/DatabaseName").toString();
    } else {
        //QString fbfilelocation = "Service_new";
        QString fbfilelocation = QDir::currentPath().append("/db/HP.FDB");
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
        //QString fbusername = "admin";
        QString fbusername = "SYSDBA";
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

//void SetupManager::setSqliteFileLocation(QString location)
//{
//    QSettings settings; //(getSettingFile(),  QSettings::IniFormat);
//    settings.setValue("db/sqlitefilelocation", location);
//}

//QString SetupManager::getFBUserName()
//{
//    QSettings settings; //(getSettingFile(),  QSettings::IniFormat);
//    if (settings.contains("db/fbusername")){
//        return settings.value("db/fbusername").toString();
//    } else {
//        QString fbusername = "SYSDBA";
//        settings.setValue("db/fbusername", fbusername);
//        return fbusername;
//    }
//}

//void SetupManager::setFBUserName(QString name)
//{
//    QSettings settings; //(getSettingFile(),  QSettings::IniFormat);
//    settings.setValue("db/fbusername", name);
//}


//bool SetupManager::openSqliteDatabase()
//{
//    QSqlDatabase sqliteDatabase = QSqlDatabase::database(QLatin1String(QSqlDatabase::defaultConnection), false);
//    if (sqliteDatabase.isOpen()) return true;
//    sqliteDatabase = QSqlDatabase::addDatabase("QSQLITE");
//    sqliteDatabase.setDatabaseName(getSqliteFileLocation());
//    sqliteStatus = sqliteDatabase.open();
//    if (!sqliteStatus) return false;
//    sqliteStatus = false;
//    QSqlQuery q(sqliteDatabase);
//    q.exec("PRAGMA foreign_keys = ON;");
//    q.exec("SELECT VERSION FROM DB_VERSION");
//    if (!q.lastError().isValid()){
//        if (q.next()) {
//            if (q.value(0).toString() == QString("0.0.3"))
//                sqliteStatus = true;
//        }
//    }
//    if (!sqliteStatus) sqliteDatabase.close();
//    return sqliteStatus;
//}

//int SetupManager::openFireBirdSQLDatabase()
//{
//    QSqlDatabase fireBirdSQLDatabase =  QSqlDatabase::database(QLatin1String("fb"), false);
//    if (fireBirdSQLDatabase.isOpen()) return FBCorrect;
//    fireBirdSQLDatabase = QSqlDatabase::addDatabase("QFIREBIRD", QLatin1String("fb"));
//    fireBirdSQLDatabase.setDatabaseName(getFBFileLocation());
//    fireBirdSQLDatabase.setHostName(getFBHostName());
//    fireBirdSQLDatabase.setUserName(getFBUserName());
//    fireBirdSQLDatabase.setPassword(getFBPassword());
//    fireBirdSQLDatabase.setConnectOptions("CHARSET=UTF-8");
//    if  (!fireBirdSQLDatabase.open()) {
//        fbStatus =  FBCantOpen;
//        return fbStatus;
//    }
//    fbStatus = FBWrongVersion;
//    QSqlQuery q(fireBirdSQLDatabase);
//    q.exec("SELECT VERSION FROM DB_VERSION");
//    if (!q.lastError().isValid()){
//        if (q.next()) {
//            if (q.value(0).toString() == QString("0.0.3"))
//                fbStatus = FBCorrect;
//        }
//    }
//    if (fbStatus != FBCorrect) fireBirdSQLDatabase.close();
//    qDebug() << fireBirdSQLDatabase.driver()->subscribedToNotifications();
//    return fbStatus;
//}

int SetupManager::openSQLDatabase(QString connectionName)
{   
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
//            fbStatus = FBWrongVersion;
//            QSqlQuery q(fireBirdSQLDatabase);
//            q.exec("SELECT VERSION FROM DB_VERSION");
//            if (!q.lastError().isValid()){
//                if (q.next()) {
//                    if (q.value(0).toString() == QString("0.0.3"))
//                        fbStatus = FBCorrect;
//                }
//            }
    if (dbStatus != FBCorrect) fireBirdSQLDatabase.close();

    return dbStatus;
    /* MYSQL */
    /*    QSqlDatabase mySQLDatabase =  QSqlDatabase::database(connectionName.toLatin1(), false);
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

    //    dbStatus = FBWrongVersion;
    //    QSqlQuery q(mySQLDatabase);
    //    q.exec("SELECT VERSION FROM DB_VERSION");
    //    if (!q.lastError().isValid()){
    //        if (q.next()) {
    //            if (q.value(0).toString() == QString("0.0.3"))
    //                dbStatus = FBCorrect;
    //        }
    //    }

    if (dbStatus != FBCorrect) mySQLDatabase.close();
    return dbStatus;*/
}

//QSqlDatabase SetupManager::getSqliteDatabase()
//{
//    return QSqlDatabase::database(QLatin1String(QSqlDatabase::defaultConnection), false);
//}

QSqlDatabase SetupManager::getDatabase(const QString &connectionName)
{    
    return QSqlDatabase::database(connectionName);
}

//QSqlDatabase SetupManager::getFireBirdSQLDatabase()
//{
//    return QSqlDatabase::database(QLatin1String("fb"), false);
//}

bool SetupManager::getSqlQueryForDB(QSqlQuery &q, const QString &connectionName)
{
    QSqlDatabase db = QSqlDatabase::database(connectionName);
    if (!db.isOpen())
        return false;
    q = QSqlQuery(db);
    return true;
}

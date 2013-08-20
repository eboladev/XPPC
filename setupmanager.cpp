#include "setupmanager.h"

#include <QSettings>
#include <QDebug>
#include <QApplication>

#include "applicationupdatedialog.h"

SetupManager::SetupManager(QObject *parent) :
    QObject(parent),
    dbStatus(FBCantOpen)
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

void SetupManager::setCurrentBranch(const QVariant &id)
{
    QSettings settings;
    settings.setValue("db/CurrentBranch",id);
}

int SetupManager::getCurrentBranch() const
{
    QSettings settings;
    if (settings.contains("db/CurrentBranch"))
        return settings.value("db/CurrentBranch").toInt();
    else
        return 0;
}

void SetupManager::setClientNameCompleterEnabled(const bool &enabled)
{
    QSettings settings;
    settings.setValue("db/ClientNameCompleter", enabled);
}

bool SetupManager::getClientNameCompleterEnabled()
{
    QSettings settings; //(getSettingFile(),  QSettings::IniFormat);
    if (settings.contains("db/ClientNameCompleter")){
        return settings.value("db/ClientNameCompleter").toBool();
    } else {
        settings.setValue("db/ClientNameCompleter", true);
        return true;
    }
}

QString SetupManager::getFBFileLocation()
{
    QSettings settings; //(getSettingFile(),  QSettings::IniFormat);
    if (settings.contains("db/DatabaseName")){
        return settings.value("db/DatabaseName").toString();
    } else {
        QString fbfilelocation = "hpnew";
        settings.setValue("db/DatabaseName", fbfilelocation);
        return fbfilelocation;
    }
}

void SetupManager::setFBFileLocation(QString location)
{
    QSettings settings; //(getSettingFile(),  QSettings::IniFormat);    
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
        QString fbfilelocation = "hpnew";
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
        QString fbusername = "postgres";
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
        QString fbusername = "5432";
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
    
    //dbStatus = FBCorrect;
    
    fireBirdSQLDatabase =  QSqlDatabase::database(connectionName.toLatin1(), false);
    if (fireBirdSQLDatabase.isOpen()) return FBCorrect;
    fireBirdSQLDatabase = QSqlDatabase::addDatabase("QPSQL", connectionName.toLatin1());
    fireBirdSQLDatabase.setDatabaseName(getDbName());
    fireBirdSQLDatabase.setHostName(getDbHostName());
    fireBirdSQLDatabase.setUserName(getDbUserName());
    fireBirdSQLDatabase.setPassword(getDbPassword());    
    if  (!fireBirdSQLDatabase.open()) {
        dbStatus =  FBCantOpen;
        return dbStatus;
    }
    else
        dbStatus = FBCorrect;
    
    /*QSqlQuery q(fireBirdSQLDatabase);

    q.exec("select version, changelog, link from application_updates order by id desc limit 1");

    if (q.next())
    {
        if (q.value(0).toString() != qApp->applicationVersion())
        {
            ApplicationUpdateDialog aud;
            aud.setDownloadLink(q.value(2).toString());
            aud.setVersion(q.value(0).toString());
            aud.setChangelogtext(q.value(1).toString());
            //if (aud.exec())

            aud.exec();
                exit(0);//qApp->closingDown();
            //if (!aud.exec())
               // exit(0);
        }
    }*/

    qDebug() << Q_FUNC_INFO << fireBirdSQLDatabase.driver()->hasFeature(QSqlDriver::EventNotifications);
    if (dbStatus != FBCorrect) fireBirdSQLDatabase.close();    
    return dbStatus;
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

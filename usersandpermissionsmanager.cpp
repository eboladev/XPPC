#include "usersandpermissionsmanager.h"
#include "changeuserdialog.h"
#include "setupmanager.h"

#include <QCryptographicHash>
#include <QTimer>
#include <QDebug>
#include <QSettings>

const int USERTIMEOUT = 600000;

UsersAndPermissionsManager::UsersAndPermissionsManager(QObject *parent) :
    QObject(parent),
    permissions(0),
    currentUserId(-1)
{
    connect(changeUserDialog, SIGNAL(userLogOut()), SLOT(onUserLogOut()));
    userActivityTimer = new QTimer(this);
    connect(userActivityTimer, SIGNAL(timeout()), changeUserDialog, SLOT(onLogout()));
    connect(userActivityTimer, SIGNAL(timeout()), this, SLOT(showChangeUserDialog()));
}

Q_GLOBAL_STATIC(UsersAndPermissionsManager, singlton_instance)
UsersAndPermissionsManager *UsersAndPermissionsManager::instance()
{
    return singlton_instance();
}

bool UsersAndPermissionsManager::isCanAddTicket()
{
    return permissions & PAddTicket;
}

bool UsersAndPermissionsManager::isCanCloseTicket()
{
    return permissions & PCloseticket;
}

bool UsersAndPermissionsManager::isCanMoveToWork()
{
    return permissions & PMoveToWork;
}

bool UsersAndPermissionsManager::isCanMoveToReady()
{
    return permissions & PMoveToReady;
}

bool UsersAndPermissionsManager::isCanGuaranteeReturn()
{
    return permissions & PGuaranteeReturn;
}

bool UsersAndPermissionsManager::isCanEditJobList()
{
    return permissions & PJobListEdit;
}

bool UsersAndPermissionsManager::isCanEditBranches()
{
    return permissions & PEditBranches;
}

bool UsersAndPermissionsManager::isCanEditUsers()
{
    return permissions & PEditUsers;
}

bool UsersAndPermissionsManager::isCanEditPermissions()
{
    return permissions & PEditPermissions;
}

bool UsersAndPermissionsManager::isCanEditClosedTickets()
{
    return permissions & PEditClosedTickets;
}

bool UsersAndPermissionsManager::isCanEditSalary()
{
    return permissions & PEditSalary;
}

bool UsersAndPermissionsManager::isUserLoggedIn()
{
    return currentUserId != -1;
}

void UsersAndPermissionsManager::showChangeUserDialog()
{   
    changeUserDialog->setUser(getLastUserLogin());
    if (changeUserDialog->exec())
        changeUser(changeUserDialog->getUser(),changeUserDialog->getPassword());
}

void UsersAndPermissionsManager::onUserLogOut()
{
    setCurrentUser("");
    setCurrentUserId(-1);
    setPermissions(0);
    userActivityTimer->stop();
    emit userLogOut();
}

void UsersAndPermissionsManager::onUserLogIn()
{
    qDebug() << Q_FUNC_INFO << getCurrentUser();
    accessManager->startUserActivityTimer();
    changeUserDialog->onSuccesfullLogin();
    qDebug() << changeUserDialog->getUser();
    setLastUserLogin(changeUserDialog->getUser());
    emit userLogIn();
}

void UsersAndPermissionsManager::changeUser(const QString &login, const QString &password)
{
    qDebug() << login << password;
    QString passwordHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha1).toHex();

    QSqlQuery q;
    if (!SetupManager::instance()->getSqlQueryForDB(q))
        return;    //permissions
    q.prepare("select password, employee_fio, employee_id, permissions from groups "
              "join employee on (employee.group_id = groups.id) where login = ? and fired = FALSE");
    q.addBindValue(login);
    if (!q.exec())
        qDebug() << q.lastQuery() << q.lastError();

    if (!q.next())
    {
        emit failedToLogin(trUtf8("Пользователь %0 не найден").arg(login));
        qDebug() << q.lastQuery() << login;
        return;
    }


    QString fio = q.value(1).toString();
    QString passwordB = q.value(0).toString().remove(0,2); //remove the \x escape character(thanks for postgress for adding it >_>)
    QVariant currentUserId = q.value(2);

    if (passwordHash != passwordB)
    {
        emit failedToLogin(trUtf8("Пароль введен неправильно"));
        return;
    }

    setCurrentUser(login);
    setPermissions(q.value(3).toInt());
    setCurrentUserId(currentUserId);
    setCurrentUserDisplayName(fio);
    emit changeUserDisplayName();

    onUserLogIn();
}

QString UsersAndPermissionsManager::getCurrentUserDisplayName() const
{
    return currentUserDisplayName;
}

void UsersAndPermissionsManager::setCurrentUserDisplayName(const QString &value)
{
    currentUserDisplayName = value;
}

void UsersAndPermissionsManager::setPermissions(const int &value)
{
    permissions = value;
}

QString UsersAndPermissionsManager::getLastUserLogin() const
{
    QSettings settings;
    return settings.value("user/lastLogin","").toString();
}

void UsersAndPermissionsManager::setLastUserLogin(const QString &value)
{
    QSettings settings;
    settings.setValue("user/lastLogin", value);
}

void UsersAndPermissionsManager::startUserActivityTimer()
{
    userActivityTimer->start(USERTIMEOUT);
}

QString UsersAndPermissionsManager::getCurrentUser()
{
    return currentUser;
}

QVariant UsersAndPermissionsManager::getCurrentUserId() const
{
    return currentUserId;
}

void UsersAndPermissionsManager::setCurrentUser(QString user)
{
    currentUser = user;
}

void UsersAndPermissionsManager::setCurrentUserId(const QVariant &id)
{
    currentUserId = id;
}

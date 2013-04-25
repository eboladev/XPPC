#ifndef USERSANDPERMISSIONSMANAGER_H
#define USERSANDPERMISSIONSMANAGER_H

#include <QObject>
#include <QVariant>

#define accessManager (static_cast<UsersAndPermissionsManager *>(UsersAndPermissionsManager::instance()))

class QTimer;

class UsersAndPermissionsManager : public QObject
{
    Q_OBJECT
public:

    enum AccessPermissions
    {
        PAddTicket = 0x01,
        PCloseticket = 0x02,
        PMoveToWork = 0x04,
        PMoveToReady = 0x08,
        PGuaranteeReturn = 0x20,
        PJobListEdit = 0x40,
        PEditBranches = 0x80,
        PEditUsers = 0x100,
        PEditPermissions = 0x200,
        PEditClosedTickets = 0x400,
        PEditSalary = 0x800
    };

public:
    explicit UsersAndPermissionsManager(QObject *parent = 0);
    static UsersAndPermissionsManager* instance();

    bool isCanAddTicket();
    bool isCanCloseTicket();
    bool isCanMoveToWork();
    bool isCanMoveToReady();
    bool isCanGuaranteeReturn();
    bool isCanEditJobList();
    bool isCanEditBranches();
    bool isCanEditUsers();
    bool isCanEditPermissions();
    bool isCanEditClosedTickets();
    bool isCanEditSalary();

    bool isUserLoggedIn();

    int getPermissions() const {return permissions;}
    void setPermissions(const int& value);

    QString getCurrentUser();
    QVariant getCurrentUserId() const;
    void setCurrentUser(QString user);
    void setCurrentUserId(const QVariant& id);
    QString getLastUserLogin() const;
    void setLastUserLogin(const QString &value);    

    void startUserActivityTimer();

    QString getCurrentUserDisplayName() const;
    void setCurrentUserDisplayName(const QString &value);

public Q_SLOTS:
    void showChangeUserDialog();
    void onUserLogOut();
    void onUserLogIn();

private:
    void changeUser(const QString& login, const QString& password);

Q_SIGNALS:
    void userLogOut();
    void userLogIn();
    void changeUserDisplayName();
    void failedToLogin(const QString& error);

private:
    int permissions;
    QString currentUser;
    QString currentUserDisplayName;
    QVariant currentUserId;
    QTimer* userActivityTimer;
};

#endif // USERSANDPERMISSIONSMANAGER_H

#ifndef USERLOGINPASSMANAGER_H
#define USERLOGINPASSMANAGER_H

#include <QDialog>
#include <QVariant>
#include "sqlextension.h"

namespace Ui {
class UserLoginPassManager;
}

class UserLoginPassManager : public QDialog, SqlExtension
{
    Q_OBJECT
    
public:
    explicit UserLoginPassManager(const QString& dbConnectionString, QWidget *parent = 0);
    ~UserLoginPassManager();
    QString getUserLogin();
    QString getUserPassword();
    void setUserLogin(QString login);
    void setUserId(const QVariant& id);

private slots:
    void onAccept();

private:
    Ui::UserLoginPassManager *ui;
    QVariant userId;
};

#endif // USERLOGINPASSMANAGER_H

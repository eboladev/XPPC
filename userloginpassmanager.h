#ifndef USERLOGINPASSMANAGER_H
#define USERLOGINPASSMANAGER_H

#include <QDialog>
#include <QVariant>

namespace Ui {
class UserLoginPassManager;
}

class UserLoginPassManager : public QDialog
{
    Q_OBJECT
    
public:
    explicit UserLoginPassManager(QWidget *parent = 0);
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

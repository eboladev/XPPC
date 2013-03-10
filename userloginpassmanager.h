#ifndef USERLOGINPASSMANAGER_H
#define USERLOGINPASSMANAGER_H

#include <QDialog>

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

private slots:
    void onAccept();

private:
    Ui::UserLoginPassManager *ui;
};

#endif // USERLOGINPASSMANAGER_H

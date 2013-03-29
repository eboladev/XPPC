#ifndef CHANGEUSERDIALOG_H
#define CHANGEUSERDIALOG_H

#include <QDialog>

namespace Ui {
class ChangeUserDialog;
}

class ChangeUserDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ChangeUserDialog(QWidget *parent = 0);
    ~ChangeUserDialog();
    QString getUser();
    void setUser(const QString& userLogin);
    QString getPassword();

public slots:
    void onSuccesfullLogin();
    void onLogout();
    bool exec();

signals:
    void userLogOut();

private:
    Ui::ChangeUserDialog *ui;
};

#endif // CHANGEUSERDIALOG_H

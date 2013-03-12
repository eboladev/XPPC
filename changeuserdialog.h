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
    QString getPassword();

public slots:
    void onSuccesfullLogin();

private slots:
    void onAccept();
    void onLogout();

signals:
    void changePermissions();

private:
    Ui::ChangeUserDialog *ui;
};

#endif // CHANGEUSERDIALOG_H
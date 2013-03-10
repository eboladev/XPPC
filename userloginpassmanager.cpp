#include "userloginpassmanager.h"
#include "ui_userloginpassmanager.h"
#include "setupmanager.h"

#include <QMessageBox>

UserLoginPassManager::UserLoginPassManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserLoginPassManager)
{
    ui->setupUi(this);
    connect(ui->pushButtonOk, SIGNAL(clicked()), SLOT(onAccept()));
    connect(ui->pushButtonCancel, SIGNAL(clicked()), SLOT(reject()));
}

UserLoginPassManager::~UserLoginPassManager()
{
    delete ui;
}

QString UserLoginPassManager::getUserLogin()
{
    return ui->lineEditLogin->text();
}

QString UserLoginPassManager::getUserPassword()
{
    return ui->lineEditPassword->text();
}

void UserLoginPassManager::setUserLogin(QString login)
{
    ui->lineEditLogin->setText(login);
}

void UserLoginPassManager::onAccept()
{
    if ((ui->lineEditPassword->text() == ui->lineEditPasswordConfirm->text())
            && !ui->lineEditPassword->text().isEmpty()
            && !ui->lineEditLogin->text().isEmpty())
    {
        QSqlQuery q;
        if (!SetupManager::instance()->getSqlQueryForDB(q))
            return;
        q.prepare("select employee_id from employee where login = ?");
        q.addBindValue(ui->lineEditLogin->text());
        q.exec();
        if (q.next())
        {
            QMessageBox::information(this, trUtf8("Ошибка!"), trUtf8("Такой логин уже существует."));
            return;
        }
        accept();
    }
    else
        QMessageBox::information(this, trUtf8("Ошибка!"), trUtf8("Проверьте введенные данные."));
}

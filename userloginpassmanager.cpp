#include "userloginpassmanager.h"
#include "ui_userloginpassmanager.h"
#include "setupmanager.h"

#include <QMessageBox>
#include <QCryptographicHash>

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

void UserLoginPassManager::setUserId(const QVariant &id)
{
    userId = id;
}

void UserLoginPassManager::onAccept()
{
    if ((ui->lineEditPassword->text() == ui->lineEditPasswordConfirm->text())
            && !ui->lineEditPassword->text().isEmpty()
            && !ui->lineEditLogin->text().isEmpty())
    {
        QSqlQuery q;
        if (!setupManager->getSqlQueryForDB(q))
            return;
        q.prepare("select employee_id from employee where login = ?");
        q.addBindValue(ui->lineEditLogin->text());
        q.exec();
        qDebug() << userId;
        while (q.next())
        {
            if (q.value(0) != userId)
            {
                QMessageBox::information(this, trUtf8("Ошибка!"), trUtf8("Такой логин уже существует."));
                return;
            }
        }

        q.prepare("update employee set login = ?, password = ? where employee_id = ?");
        q.addBindValue(getUserLogin());
        q.addBindValue(QCryptographicHash::hash(getUserPassword().toUtf8(), QCryptographicHash::Sha1));
        q.addBindValue(userId);
        if (!q.exec())
            qDebug() << q.lastError() << q.lastQuery();
        accept();
    }
    else
        QMessageBox::information(this, trUtf8("Ошибка!"), trUtf8("Проверьте введенные данные."));
}

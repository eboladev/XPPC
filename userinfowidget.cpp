#include "userinfowidget.h"
#include "ui_userinfowidget.h"

#include "setupmanager.h"
#include "userloginpassmanager.h"

#include <QMessageBox>
#include <QDebug>

UserInfoWidget::UserInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserInfoWidget)
{
    ui->setupUi(this);
    connect(ui->lineEditName, SIGNAL(textChanged(QString)), this, SIGNAL(userNameChanged(QString)));
    connect(ui->pushButtonSave, SIGNAL(clicked()), this, SLOT(onSaveChanges()));
    connect(ui->pushButtonPassowrdRecovery, SIGNAL(clicked()), this, SLOT(onChangeLoginpass()));
}

UserInfoWidget::~UserInfoWidget()
{
    delete ui;
}

void UserInfoWidget::setUserId(const QVariant &id)
{
    userId = id;
}

void UserInfoWidget::setUserName(const QString &name)
{
    blockSignals(true);
    ui->lineEditName->setText(name);
    blockSignals(false);
}

void UserInfoWidget::setUserPhone(const QString &phone)
{
    ui->lineEditPhone->setText(phone);
}

void UserInfoWidget::setUserLogin(const QString &login)
{
    ui->lineEditLogin->setText(login);
}

void UserInfoWidget::setUserRate(const int &rate)
{
    ui->spinBoxRate->setValue(rate);
}

void UserInfoWidget::setUserPercent(const int &percent)
{
    ui->spinBoxPercent->setValue(percent);
}

void UserInfoWidget::setUserSalePercent(const int &sale_percent)
{
    ui->spinBoxSalePercent->setValue(sale_percent);
}

void UserInfoWidget::setUserSalaryPerDay(const int &spDay)
{
    ui->spinBoxSPday->setValue(spDay);
}

QVariant UserInfoWidget::getUserId() const
{
    return userId;
}

QString UserInfoWidget::getUserName() const
{
    return ui->lineEditName->text().trimmed();
}

QString UserInfoWidget::getUserPhone() const
{
    return ui->lineEditPhone->text().trimmed();
}

QString UserInfoWidget::getUserLogin() const
{
    return ui->lineEditLogin->text().trimmed();
}

int UserInfoWidget::getUserRate() const
{
    return ui->spinBoxRate->value();
}

int UserInfoWidget::getUserPercent() const
{
    return ui->spinBoxPercent->value();
}

int UserInfoWidget::getUserSalePercent() const
{
    return ui->spinBoxSalePercent->value();
}

int UserInfoWidget::getUserSalaryPerDay() const
{
    return ui->spinBoxSPday->value();
}

void UserInfoWidget::onSaveChanges()
{
    QSqlQuery q;
    if (!setupManager->getSqlQueryForDB(q))
        return;

    q.prepare("update employee set employee_fio = ?, phone = ?, "
              "employee_rate = ?, employee_percent = ?, employee_sale_percent = ?, "
              "employee_salaryperday = ? where employee_id = ?");
    q.addBindValue(getUserName());
    q.addBindValue(getUserPhone());
    q.addBindValue(getUserRate());
    q.addBindValue(getUserPercent());
    q.addBindValue(getUserSalePercent());
    q.addBindValue(getUserSalaryPerDay());
    q.addBindValue(getUserId());
    q.exec();
    emit changesSaved();
}

void UserInfoWidget::onChangeLoginpass()
{
    UserLoginPassManager ulpm;
    ulpm.setUserLogin(getUserLogin());
    ulpm.setUserId(getUserId());
    if (ulpm.exec())
    {
        setUserLogin(ulpm.getUserLogin());
        emit changesSaved();
    }
}

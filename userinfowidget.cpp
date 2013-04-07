#include "userinfowidget.h"
#include "ui_userinfowidget.h"

#include "userloginpassmanager.h"

#include <QStandardItemModel>
#include <QMessageBox>
#include <QDebug>

UserInfoWidget::UserInfoWidget(const QString &dbConnectionString, QWidget *parent) :
    QWidget(parent),
    SqlExtension(dbConnectionString),
    m_dbConnectionString(dbConnectionString),
    ui(new Ui::UserInfoWidget)
{
    ui->setupUi(this);
    connect(ui->lineEditName, SIGNAL(textChanged(QString)), this, SIGNAL(userNameChanged(QString)));
    connect(ui->pushButtonSave, SIGNAL(clicked()), this, SIGNAL(changesSaved()));
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

void UserInfoWidget::setCurrentUserGroup(const QVariant &group_id)
{
    ui->comboBoxUserGroup->setCurrentIndex(ui->comboBoxUserGroup->findData(group_id, Qt::UserRole + 2));
}

void UserInfoWidget::setGroupsEditable(const bool &isEditable)
{
    ui->comboBoxUserGroup->setEnabled(isEditable);
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

QVariant UserInfoWidget::getCurrentGroupId() const
{
    qDebug() << Q_FUNC_INFO;
    qDebug() << ui->comboBoxUserGroup->itemData(ui->comboBoxUserGroup->currentIndex(),Qt::UserRole + 2);;
    return ui->comboBoxUserGroup->itemData(ui->comboBoxUserGroup->currentIndex(),Qt::UserRole + 2);
}

void UserInfoWidget::setItemModelForGroupsComboBox(QStandardItemModel *model)
{
    ui->comboBoxUserGroup->setModel(model);
}

void UserInfoWidget::onChangeLoginpass()
{
    UserLoginPassManager* ulpm = new UserLoginPassManager(m_dbConnectionString, this);
    ulpm->setUserLogin(getUserLogin());
    ulpm->setUserId(getUserId());
    if (ulpm->exec())
    {
        setUserLogin(ulpm->getUserLogin());
        //emit changesSaved();
    }
}

#include "usermanagementdialog.h"
#include "ui_usermanagementdialog.h"
#include "userloginpassmanager.h"
#include "setupmanager.h"
#include "employeeitemmodel.h"

#include <QStandardItemModel>
#include <QMenu>
#include <QDateTimeEdit>
#include <QSortFilterProxyModel>

UserManagementDialog::UserManagementDialog(const QString &dbConnectionString, QWidget *parent) :
    QDialog(parent),
    SqlExtension(dbConnectionString),
    ui(new Ui::UserManagementDialog)
{
    ui->setupUi(this);
    employeeModel = new EmployeeItemModel(dbConnectionString,this);
    employeeProxyModel = new QSortFilterProxyModel(this);
    employeeProxyModel->setSourceModel(employeeModel);
    ui->treeViewUsers->setModel(employeeProxyModel);
    connect(ui->pushButtonOk, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->pushButtonCancel, SIGNAL(clicked()), SLOT(reject()));
    connect(ui->radioButtonFired, SIGNAL(clicked()), SLOT(refreshModel()));
    connect(ui->radioButtonWorking, SIGNAL(clicked()), SLOT(refreshModel()));    
    connect(ui->lineEditFilter, SIGNAL(textChanged(QString)), employeeProxyModel, SLOT(setFilterFixedString(QString)));
    connect(ui->pushButtonAdd, SIGNAL(clicked()), this, SLOT(onAddEmployee()));
    connect(ui->pushButtonFire, SIGNAL(clicked()), this, SLOT(onFireEmployee()));
    connect(ui->treeViewUsers->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(onCurrentEmployeeChanges(QModelIndex,QModelIndex)));
    connect(ui->widgetUserInfo, SIGNAL(userNameChanged(QString)), this, SLOT(onUserNameChanged(QString)));
    connect(ui->widgetUserInfo, SIGNAL(changesSaved()), this, SLOT(onUserInfoChangesSaved()));
    ui->treeViewUsers->setContextMenuPolicy(Qt::CustomContextMenu);   
    connect(ui->treeViewUsers, SIGNAL(customContextMenuRequested(QPoint)), SLOT(onCustomContextMenuRequested(QPoint)));
    refreshModel();
}

UserManagementDialog::~UserManagementDialog()
{
    delete ui;
}

void UserManagementDialog::onCustomContextMenuRequested(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);
    connect(menu, SIGNAL(aboutToHide()), menu, SLOT(deleteLater()));
    QModelIndex ind = ui->treeViewUsers->indexAt(pos);
    if (ui->radioButtonWorking->isChecked())
        menu->addAction(trUtf8("Добавить"), this, SLOT(onAddEmployee()));
    if (ind.isValid())
    {
#ifdef DEBUG
        if (ui->radioButtonWorking->isChecked())
            menu->addAction(trUtf8("Уволить"), this, SLOT(onFireEmployee()));
#endif
        if (ui->radioButtonFired->isChecked())
            menu->addAction(trUtf8("Восстановить"), this, SLOT(onFireEmployee()));
#ifdef RELEASE
        if (getItemFromIndex(ui->treeViewUsers->currentIndex())->data(isPasswordSetRole).toBool())
        {
            if (SetupManager::instance()->getCurrentUser() == getItemFromIndex(ui->treeViewUsers->currentIndex())->data(LoginRole).toString())
                menu->addAction(trUtf8("Логин\\пароль"),this, SLOT(onChangeLoginpass()));
        }
        else
#endif
            menu->addAction(trUtf8("Логин\\пароль"),this, SLOT(onChangeLoginpass()));
    }
    menu->exec(QCursor::pos());
}

void UserManagementDialog::onCurrentEmployeeChanges(QModelIndex current, QModelIndex)
{
    ui->widgetUserInfo->setEnabled(false);
    if (!current.isValid())
        return;
    else
    {
#ifdef RELEASE
        if (getItemFromIndex(ui->treeViewUsers->currentIndex())->data(isPasswordSetRole).toBool())
        {
            if (SetupManager::instance()->getCurrentUser() == getItemFromIndex(ui->treeViewUsers->currentIndex())->data(LoginRole).toString())
                ui->widgetUserInfo->setEnabled(true);
        }
        else
#endif
         ui->widgetUserInfo->setEnabled(true);
    }
    ui->widgetUserInfo->setUserId(getItemFromIndex(current)->data(IDrole));
    ui->widgetUserInfo->setUserName(getItemFromIndex(current)->text());
    ui->widgetUserInfo->setUserPhone(getItemFromIndex(current)->data(PhoneRole).toString());
    ui->widgetUserInfo->setUserLogin(getItemFromIndex(current)->data(LoginRole).toString());
    ui->widgetUserInfo->setUserRate(getItemFromIndex(current)->data(RateRole).toInt());
    ui->widgetUserInfo->setUserPercent(getItemFromIndex(current)->data(PercentRole).toInt());
    ui->widgetUserInfo->setUserSalePercent(getItemFromIndex(current)->data(SalePercentRole).toInt());
    ui->widgetUserInfo->setUserSalaryPerDay(getItemFromIndex(current)->data(SalaryPerDayRole).toInt());
}

void UserManagementDialog::onAddEmployee()
{
    if (employeeModel->addEmployee(true))
    {
        ui->treeViewUsers->setCurrentIndex(employeeModel->item(employeeModel->rowCount() - 1 ,0)->index());
        ui->widgetUserInfo->setEnabled(true);//permissions required
    }
}

void UserManagementDialog::onFireEmployee()
{
    employeeModel->onFireEmployee(ui->treeViewUsers->currentIndex());
    if (employeeModel->rowCount() > 0)
        ui->treeViewUsers->setCurrentIndex(employeeModel->item(0,0)->index());
    else
        ui->widgetUserInfo->setEnabled(false);
    refreshModel();
}

void UserManagementDialog::refreshModel()
{    
    employeeModel->refreshModel(ui->radioButtonFired->isChecked(),true);

    for (int i = 0; i < employeeModel->columnCount(); ++i)
        ui->treeViewUsers->resizeColumnToContents(i);
}

void UserManagementDialog::onChangeLoginpass()
{
    UserLoginPassManager ulpm;
    ulpm.setUserLogin(ui->widgetUserInfo->getUserLogin());
    ulpm.setUserId(ui->widgetUserInfo->getUserId());
    if (ulpm.exec())    
        refreshModel();    
}

void UserManagementDialog::onUserNameChanged(QString name)
{
    if (ui->treeViewUsers->currentIndex().isValid())
        employeeModel->itemFromIndex(employeeProxyModel->mapToSource(ui->treeViewUsers->currentIndex()))->setText(name);
}

void UserManagementDialog::onUserInfoChangesSaved()
{
    if (!ui->treeViewUsers->currentIndex().isValid())
        return;
    getItemFromIndex(ui->treeViewUsers->currentIndex())->setData(ui->widgetUserInfo->getUserId(),IDrole);
    getItemFromIndex(ui->treeViewUsers->currentIndex())->setData(ui->widgetUserInfo->getUserName(),NameRole);
    getItemFromIndex(ui->treeViewUsers->currentIndex())->setData(ui->widgetUserInfo->getUserPhone(), PhoneRole);
    getItemFromIndex(ui->treeViewUsers->currentIndex())->setData(ui->widgetUserInfo->getUserRate(), RateRole);
    getItemFromIndex(ui->treeViewUsers->currentIndex())->setData(ui->widgetUserInfo->getUserPercent(), PercentRole);
    getItemFromIndex(ui->treeViewUsers->currentIndex())->setData(ui->widgetUserInfo->getUserSalePercent(), SalePercentRole);
    getItemFromIndex(ui->treeViewUsers->currentIndex())->setData(ui->widgetUserInfo->getUserSalaryPerDay(), SalaryPerDayRole);
    getItemFromIndex(ui->treeViewUsers->currentIndex())->setData(ui->widgetUserInfo->getUserLogin(), LoginRole);
}

QStandardItem *UserManagementDialog::getItemFromIndex(QModelIndex index)
{
    if (index.model() == employeeModel)
        return employeeModel->itemFromIndex(index);
    else if (index.model() == employeeProxyModel)
        return employeeModel->itemFromIndex(employeeProxyModel->mapToSource(index));
}

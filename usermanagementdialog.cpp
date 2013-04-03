#include "usermanagementdialog.h"
#include "ui_usermanagementdialog.h"
#include "userloginpassmanager.h"
#include "setupmanager.h"
#include "employeeitemmodel.h"
#include "usersandpermissionsmanager.h"

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
    groupsModel = new QStandardItemModel(this);
    employeeProxyModel = new QSortFilterProxyModel(this);
    employeeProxyModel->setSourceModel(employeeModel);
    ui->widgetUserInfo->setItemModelForGroupsComboBox(groupsModel);
    ui->listViewGroups->setModel(groupsModel);
    ui->treeViewUsers->setModel(employeeProxyModel);
    ui->treeViewUsers->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeViewUsers->setContextMenuPolicy(Qt::CustomContextMenu);
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

    ui->listViewGroups->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listViewGroups->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->widgetPermissions, SIGNAL(setPermissions(int)), this, SLOT(onEditGroupPermissions(int)));
    connect(this, SIGNAL(permissons(int)), ui->widgetPermissions, SLOT(onSetPermissions(int)));
    connect(ui->pushButtonAddGroup, SIGNAL(clicked()), this, SLOT(onAddGroup()));
    connect(ui->listViewGroups->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(onCurrentGroupChanged(QModelIndex,QModelIndex)));
    connect(ui->listViewGroups, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(onCustomGroupsContextMenuRequested(QPoint)));
    ui->tabWidgetUsersPermissions->setTabEnabled(1,accessManager->isCanEditPermissions());
    refreshModel();
    refreshGroups();
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
        if (accessManager->isCanEditUsers())
        {
            if (ui->radioButtonWorking->isChecked())
                menu->addAction(trUtf8("Уволить"), this, SLOT(onFireEmployee()));

            if (ui->radioButtonFired->isChecked())
                menu->addAction(trUtf8("Восстановить"), this, SLOT(onFireEmployee()));
        }
#ifdef RELEASE
        if (getItemFromIndex(ui->treeViewUsers->currentIndex())->data(isPasswordSetRole).toBool())
        {
            if (accessManager->getCurrentUser() == getItemFromIndex(ui->treeViewUsers->currentIndex())->data(LoginRole).toString()
                    ||
                    accessManager->isCanEditUsers())
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
    ui->widgetUserInfo->setGroupsEditable(accessManager->isCanEditPermissions());
    if (!current.isValid())
        return;
    else
    {
#ifdef RELEASE
        if (getItemFromIndex(ui->treeViewUsers->currentIndex())->data(isPasswordSetRole).toBool())
        {
            if (accessManager->getCurrentUser() == getItemFromIndex(ui->treeViewUsers->currentIndex())->data(LoginRole).toString()
                    ||
                    accessManager->isCanEditUsers())
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
    ui->widgetUserInfo->setCurrentUserGroup(getItemFromIndex(current)->data(GroupIdRole));
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
    getItemFromIndex(ui->treeViewUsers->currentIndex())->setText(name);
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
    getItemFromIndex(ui->treeViewUsers->currentIndex())->setData(ui->widgetUserInfo->getCurrentGroupId(), GroupIdRole);

    QSqlQuery q;
    if (!setupManager->getSqlQueryForDB(q))
        return;
    q.prepare("update employee set employee_fio = ?, phone = ?, "
              "employee_rate = ?, employee_percent = ?, employee_sale_percent = ?, "
              "employee_salaryperday = ?, group_id = ? where employee_id = ?");
    q.addBindValue(ui->widgetUserInfo->getUserName());
    q.addBindValue(ui->widgetUserInfo->getUserPhone());
    q.addBindValue(ui->widgetUserInfo->getUserRate());
    q.addBindValue(ui->widgetUserInfo->getUserPercent());
    q.addBindValue(ui->widgetUserInfo->getUserSalePercent());
    q.addBindValue(ui->widgetUserInfo->getUserSalaryPerDay());
    q.addBindValue(ui->widgetUserInfo->getCurrentGroupId());
    q.addBindValue(ui->widgetUserInfo->getUserId());
    if (!q.exec())
        qDebug() << q.lastError() << q.lastQuery();
}

void UserManagementDialog::onEditGroupPermissions(const int &permissions)
{
    qDebug() << currentGroupId << Q_FUNC_INFO;
    QSqlQuery q;
    if (!setupManager->getSqlQueryForDB(q))
        return;
    q.prepare("update groups set permissions = ? where id = ?");
    q.addBindValue(permissions);
    q.addBindValue(currentGroupId);
    if (!q.exec())
        qDebug() << q.lastError() << q.lastQuery();

    refreshGroups();
}

QStandardItem *UserManagementDialog::getItemFromIndex(QModelIndex index)
{
    qDebug() << Q_FUNC_INFO;
    if (!index.isValid())
        return new QStandardItem();
    if (index.model() == employeeModel)
        return employeeModel->itemFromIndex(index);
    else if (index.model() == employeeProxyModel)
        return employeeModel->itemFromIndex(employeeProxyModel->mapToSource(index));
}

void UserManagementDialog::refreshGroups()
{
    groupsModel->clear();
    groupsModel->setHorizontalHeaderLabels(QStringList() << trUtf8("Название"));
    QSqlQuery q;
    if (!setupManager->getSqlQueryForDB(q))
        return;
    q.exec("select name,permissions,id from groups");
    while(q.next())
    {
        QStandardItem* groupName = new QStandardItem(q.value(0).toString());
        groupName->setData(q.value(1));
        groupName->setData(q.value(2), Qt::UserRole + 2); //group_id
        groupsModel->appendRow(groupName);
    }
}

void UserManagementDialog::onAddGroup()
{
    QSqlQuery q;
    if (!setupManager->getSqlQueryForDB(q))
        return;
    q.prepare("insert into groups(name,permissions) VALUES(?,?) returning id");
    q.addBindValue(ui->lineEditGroupName->text().trimmed());
    q.addBindValue(ui->widgetPermissions->getCurrentPermissions());
    if (!q.exec())
        qDebug() << q.lastError() << q.lastQuery();
    q.next();

    QStandardItem* groupName = new QStandardItem(ui->lineEditGroupName->text().trimmed());
    groupName->setData(0);
    groupsModel->appendRow(groupName);
    ui->listViewGroups->setCurrentIndex(groupsModel->item(groupsModel->rowCount() - 1 ,0)->index());
}

void UserManagementDialog::onDeleteGroup()
{
    if (!ui->listViewGroups->currentIndex().isValid())
        return;

    QSqlQuery q;
    if (!setupManager->getSqlQueryForDB(q))
        return;
    q.prepare("update employee set group_id = 1 where group_id = ?");
    q.addBindValue(groupsModel->itemFromIndex(ui->listViewGroups->currentIndex())->data());
    if (!q.exec())
        qDebug() << q.lastError() << q.lastQuery();
    q.prepare("delete from groups where id = ?");
    q.addBindValue(groupsModel->itemFromIndex(ui->listViewGroups->currentIndex())->data());
    if (!q.exec())
        qDebug() << q.lastError() << q.lastQuery();
    groupsModel->removeRow(ui->listViewGroups->currentIndex().row());
}

void UserManagementDialog::onCurrentGroupChanged(QModelIndex current, QModelIndex)
{
    currentGroupId = groupsModel->itemFromIndex(current)->data(Qt::UserRole + 2);
    emit permissons(groupsModel->itemFromIndex(current)->data().toInt());
}

void UserManagementDialog::onCustomGroupsContextMenuRequested(const QPoint &pos)
{
    qDebug() << Q_FUNC_INFO;
    QMenu *menu = new QMenu(this);
    connect(menu, SIGNAL(aboutToHide()), menu, SLOT(deleteLater()));
    QModelIndex ind = ui->listViewGroups->indexAt(pos);
    if (ind.isValid())
    {
        menu->addAction(trUtf8("Удалить группу %0").arg(ind.data().toString()),this, SLOT(onDeleteGroup()));
    }
    menu->exec(QCursor::pos());
}

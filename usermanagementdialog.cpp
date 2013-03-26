#include "usermanagementdialog.h"
#include "ui_usermanagementdialog.h"
#include "userloginpassmanager.h"
#include "setupmanager.h"
#include "employeeitemmodel.h"

#include <QStandardItemModel>
#include <QMenu>
#include <QDateEdit>
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
    connect(employeeModel, SIGNAL(itemChanged(QStandardItem*)),employeeModel, SLOT(onItemChanged(QStandardItem*)));
    connect(ui->lineEditFilter, SIGNAL(textChanged(QString)), employeeProxyModel, SLOT(setFilterFixedString(QString)));
    ui->treeViewUsers->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeViewUsers->setItemDelegateForColumn(5, new CalendarDelegate(this));
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
        qDebug() << SetupManager::instance()->getCurrentUser();
        if (SetupManager::instance()->getCurrentUser() == model->item(proxy->mapToSource(ui->treeViewUsers->currentIndex()).row(),0)->text())
#endif
            menu->addAction(trUtf8("Логин\\пароль"),this, SLOT(onChangeLoginpass()));
    }
    menu->exec(QCursor::pos());
}

void UserManagementDialog::onAddEmployee()
{
    employeeModel->addEmployee();
}

void UserManagementDialog::onFireEmployee()
{
    employeeModel->onFireEmployee(ui->treeViewUsers->currentIndex());
}

void UserManagementDialog::refreshModel()
{    
    employeeModel->refreshModel(ui->radioButtonFired->isChecked());

    for (int i = 0; i < employeeModel->columnCount(); ++i)
        ui->treeViewUsers->resizeColumnToContents(i);
}

void UserManagementDialog::onChangeLoginpass()
{
    UserLoginPassManager ulpm;
    ulpm.setUserLogin(employeeModel->item(employeeProxyModel->mapToSource(ui->treeViewUsers->currentIndex()).row(),6)->data(Qt::DisplayRole).toString());
    if (ulpm.exec())
    {
        QSqlQuery q;
        if (!getSqlQuery(q))
            return;
        q.prepare("update employee set login = ?, password = ? where employee_id = ?");
        q.addBindValue(ulpm.getUserLogin());
        q.addBindValue(QCryptographicHash::hash(ulpm.getUserPassword().toUtf8(), QCryptographicHash::Sha1));
        q.addBindValue(employeeModel->getCurrentId(ui->treeViewUsers->currentIndex()));
        if (!q.exec())
            qDebug() << q.lastError() << q.lastQuery();
        refreshModel();
    }
}

CalendarDelegate::CalendarDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget *CalendarDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QDateEdit* date = new QDateEdit(parent);
    return date;
}

void CalendarDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QDate date = index.model()->data(index,Qt::DisplayRole).toDate();
    QDateEdit *dateEdit = qobject_cast<QDateEdit*>(editor);
    dateEdit->setDate(date);
}

void CalendarDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDateEdit *dateEdit = qobject_cast<QDateEdit*>(editor);
    model->setData(index, dateEdit->date(), Qt::DisplayRole);
}

void CalendarDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

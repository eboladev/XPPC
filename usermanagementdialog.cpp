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
        if (employeeModel->item(employeeProxyModel->mapToSource(ui->treeViewUsers->currentIndex()).row(),8)->data().toBool())
        {            
            if (SetupManager::instance()->getCurrentUser() == employeeModel->item(employeeProxyModel->mapToSource(ui->treeViewUsers->currentIndex()).row(),7)->text())
                menu->addAction(trUtf8("Логин\\пароль"),this, SLOT(onChangeLoginpass()));
        }
        else
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
    QDateTimeEdit* date = new QDateTimeEdit(parent);
    return date;
}

void CalendarDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    qDebug() << index.model()->data(index,Qt::DisplayRole);
    QDateTime date;
    date.fromString(index.model()->data(index,Qt::DisplayRole).toString(),"dd-MM-yyyy_hh-mm-ss");
    qDebug() << date;
    QDateTimeEdit *dateEdit = qobject_cast<QDateTimeEdit*>(editor);
    dateEdit->setDateTime(date);
}

void CalendarDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDateTimeEdit *dateEdit = qobject_cast<QDateTimeEdit*>(editor);
    model->setData(index, dateEdit->dateTime().toString(), Qt::DisplayRole);
}

void CalendarDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

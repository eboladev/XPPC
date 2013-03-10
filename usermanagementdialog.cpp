#include "usermanagementdialog.h"
#include "ui_usermanagementdialog.h"
#include "userloginpassmanager.h"

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
    model = new QStandardItemModel(this);
    proxy = new QSortFilterProxyModel(this);
    proxy->setSourceModel(model);
    ui->treeViewUsers->setModel(proxy);
    connect(ui->pushButtonOk, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->pushButtonCancel, SIGNAL(clicked()), SLOT(reject()));
    connect(ui->radioButtonFired, SIGNAL(clicked()), SLOT(refreshModel()));
    connect(ui->radioButtonWorking, SIGNAL(clicked()), SLOT(refreshModel()));
    connect(model, SIGNAL(itemChanged(QStandardItem*)),SLOT(onItemChanged(QStandardItem*)));
    connect(ui->lineEditFilter, SIGNAL(textChanged(QString)), proxy, SLOT(setFilterFixedString(QString)));
    ui->treeViewUsers->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeViewUsers->setItemDelegateForColumn(4,new CalendarDelegate(this));
    connect(ui->treeViewUsers, SIGNAL(customContextMenuRequested(QPoint)), SLOT(onCustomContextMenuRequested(QPoint)));
    refreshModel();
}

UserManagementDialog::~UserManagementDialog()
{
    delete ui;
}

void UserManagementDialog::onItemChanged(QStandardItem *item)
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    QModelIndex idx = item->index();    
    if (idx.column() == 1 || idx.column() == 2 || idx.column() == 3 || idx.column() == 5)
        model->setData(idx,item->text().toInt(),Qt::DisplayRole);
    q.prepare("update employee set employee_fio = ?, employee_rate = ?, "
              "employee_percent = ?, employee_sale_percent = ?, "
              "employee_last_salary_date = ?, employee_salaryperday = ? "
              "where employee_id = ?");
    q.addBindValue(model->item(idx.row(),0)->data(Qt::DisplayRole));
    q.addBindValue(model->item(idx.row(),1)->data(Qt::DisplayRole).toInt());
    q.addBindValue(model->item(idx.row(),2)->data(Qt::DisplayRole).toInt());
    q.addBindValue(model->item(idx.row(),3)->data(Qt::DisplayRole).toInt());
    q.addBindValue(model->item(idx.row(),4)->data(Qt::DisplayRole));
    q.addBindValue(model->item(idx.row(),5)->data(Qt::DisplayRole).toInt());
    q.addBindValue(model->item(idx.row(),0)->data());
    if (!q.exec())
        qDebug() << q.lastError() << q.lastQuery();
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
        if (ui->radioButtonWorking->isChecked())
            menu->addAction(trUtf8("Уволить"), this, SLOT(onFireEmployee()));
        if (ui->radioButtonFired->isChecked())
            menu->addAction(trUtf8("Восстановить"), this, SLOT(onFireEmployee()));
        menu->addAction(trUtf8("Логин\\пароль"),this, SLOT(onChangeLoginpass()));
    }
    menu->exec(QCursor::pos());
}

void UserManagementDialog::onAddEmployee()
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.prepare("insert into employee(employee_fio, employee_rate, "
              "employee_percent, employee_sale_percent, "
              "employee_salaryperday) VALUES(?,?,?,?,?)");
    q.addBindValue(trUtf8("ФИО"));
    q.addBindValue(0);
    q.addBindValue(0);
    q.addBindValue(0);
    q.addBindValue(0);
    if (!q.exec())
        qDebug() << q.lastError() << q.lastQuery();
    refreshModel();
}

void UserManagementDialog::onFireEmployee()
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.prepare("update employee set fired = ? where employee_id = ?");
    q.addBindValue(ui->radioButtonWorking->isChecked());
    q.addBindValue(getCurrentId());
    if (!q.exec())
        qDebug() << q.lastError() << q.lastQuery();
    refreshModel();
}

void UserManagementDialog::refreshModel()
{
    model->clear();
    model->setHorizontalHeaderLabels(QStringList() << trUtf8("ФИО") << trUtf8("Оплата за ремонт") <<
                                      trUtf8("Процент") << trUtf8("Процент с продаж") <<
                                      trUtf8("Дата з\\п") << trUtf8("З\\п в день") <<
                                      trUtf8("Логин") << trUtf8("Пароль"));
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.prepare("select employee_id, employee_fio, employee_rate, "
              "employee_percent, employee_sale_percent, "
              "employee_last_salary_date, employee_salaryperday, "
              "login, password from employee where fired = ? ORDER BY employee_id DESC");
    q.addBindValue(ui->radioButtonFired->isChecked());
    q.exec();

    while (q.next())
    {
        QStandardItem* fio = new QStandardItem(q.value(1).toString());
        fio->setData(q.value(0));
        QStandardItem* rate = new QStandardItem(q.value(2).toString());
        rate->setToolTip(trUtf8("Оплата за ремонт по квитанции."));
        QStandardItem* percent = new QStandardItem(q.value(3).toString());
        percent->setToolTip(trUtf8("Процент с ремонта."));
        QStandardItem* sPercent = new QStandardItem(q.value(4).toString());
        sPercent->setToolTip(trUtf8("Процент с продаж."));
        QStandardItem* sDate = new QStandardItem(q.value(5).toString());
        sDate->setToolTip(trUtf8("Дата последнего получения з\\п"));
        QStandardItem* spDay = new QStandardItem(q.value(6).toString());
        spDay->setToolTip(trUtf8("Оплата за раб. день"));
        QStandardItem* login = new QStandardItem(q.value(7).toString());
        QStandardItem* pass = new QStandardItem();
        pass->setText(q.value(8).toString().isEmpty() ? trUtf8("Не задан") : trUtf8("Задан"));
        model->appendRow(QList<QStandardItem*>() << fio << rate
                         << percent << sPercent << sDate << spDay << login << pass);
    }
    for (int i = 0; i < model->columnCount(); ++i)
        ui->treeViewUsers->resizeColumnToContents(i);
}

void UserManagementDialog::onChangeLoginpass()
{
    UserLoginPassManager ulpm;
    ulpm.setUserLogin(model->item(proxy->mapToSource(ui->treeViewUsers->currentIndex()).row(),6)->data(Qt::DisplayRole).toString());
    if (ulpm.exec())
    {
        QSqlQuery q;
        if (!getSqlQuery(q))
            return;
        q.prepare("update employee set login = ?, password = ? where employee_id = ?");
        q.addBindValue(ulpm.getUserLogin());
        q.addBindValue(QCryptographicHash::hash(ulpm.getUserPassword().toUtf8(), QCryptographicHash::Sha1));
        q.addBindValue(getCurrentId());
        if (!q.exec())
            qDebug() << q.lastError() << q.lastQuery();
        refreshModel();
    }
}

QVariant UserManagementDialog::getCurrentId()
{
    return model->item(proxy->mapToSource(ui->treeViewUsers->currentIndex()).row(),0)->data();
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

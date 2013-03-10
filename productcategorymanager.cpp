#include "productcategorymanager.h"
#include "ui_productcategorymanager.h"

#include <QStandardItemModel>

ProductCategoryManager::ProductCategoryManager(const QString &dbConnectionString, QWidget *parent) :
    QDialog(parent),
    SqlExtension(dbConnectionString),
    ui(new Ui::ProductCategoryManager)
{
    ui->setupUi(this);
    model = new QStandardItemModel(this);
    connect(ui->pushButtonAdd, SIGNAL(clicked()), SLOT(onAddCategoryClicked()));
    connect(ui->pushButtonRemove, SIGNAL(clicked()), SLOT(onRemoveCategoryClicked()));
    connect(ui->pushButtonOk, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->pushButtonCancel, SIGNAL(clicked()), SLOT(reject()));
    connect(model, SIGNAL(itemChanged(QStandardItem*)), SLOT(onCurrentItemChanged(QStandardItem*)));
    ui->treeView->setModel(model);
    refreshModel();
}

ProductCategoryManager::~ProductCategoryManager()
{
    delete ui;
}

void ProductCategoryManager::onAddCategoryClicked()
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;

    q.prepare("insert into product_type(type_name) VALUES(?) returning type_id ");
    q.addBindValue(trUtf8("Название"));
    if (!q.exec())
        qDebug() << q.lastError();
    q.next();
    QStandardItem* item = new QStandardItem(trUtf8("Название"));
    item->setData(q.value(0));
    model->appendRow(item);
}

void ProductCategoryManager::onRemoveCategoryClicked()
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;

    q.prepare("delete from product_type where type_id = ? ");
    q.addBindValue(model->itemFromIndex(ui->treeView->currentIndex())->data());
    if (!q.exec())
        qDebug() << q.lastError();

    model->removeRow(ui->treeView->currentIndex().row());
}

void ProductCategoryManager::onCurrentItemChanged(QStandardItem *item)
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;

    q.prepare("update product_type set type_name = ? where type_id = ?");
    q.addBindValue(item->text());
    q.addBindValue(item->data());
    if (!q.exec())
        qDebug() << q.lastError();
}

void ProductCategoryManager::refreshModel()
{
    model->clear();
    model->setHorizontalHeaderLabels(QStringList() << trUtf8("Название"));

    QSqlQuery q;
    if (!getSqlQuery(q))
        return;

    if (!q.exec("select type_name, type_id from product_type"))
        qDebug() << q.lastError();
    while (q.next())
    {
        QStandardItem* item = new QStandardItem(q.value(0).toString());
        item->setData(q.value(1));
        model->appendRow(item);
    }
}

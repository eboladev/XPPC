#include "branchwidget.h"
#include "ui_branchwidget.h"
#include <QStandardItemModel>
BranchWidget::BranchWidget(const QString &dbConnectionString, QWidget *parent) :
    QDialog(parent),
    SqlExtension(dbConnectionString),
    ui(new Ui::BranchWidget)
{
    ui->setupUi(this);
    setWindowTitle(trUtf8("Управление филиалами"));
    model =  new QStandardItemModel(this);
    connect(ui->pushButtonOk,SIGNAL(clicked()),this,SLOT(accept()));
    connect(ui->pushButtonCancel,SIGNAL(clicked()),this,SLOT(reject()));
    connect(ui->pushButtonAddBranch,SIGNAL(clicked()),this,SLOT(onAddBranchClicked()));
    connect(ui->pushButtonRemoveBranch,SIGNAL(clicked()),this,SLOT(onRemoveBranchClicked()));
    connect(model,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(onBranchViewChanged(QStandardItem*)));
    connect(ui->pushButtonOk,SIGNAL(clicked()),this,SLOT(onCurrentBrancheChanged()));
    QSettings settings;
    getBranches();
    ui->comboBoxCurrentBranch->setCurrentIndex(settings.value("db/CurrentBranch").toInt());
}

BranchWidget::~BranchWidget()
{
    delete ui;
}

void BranchWidget::getBranches()
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;

    q.prepare("select id, branch_name, branch_description from Branch");

    if (!q.exec())
        return;
    model->clear();
    QStringList labels;
    labels << "Название" << "Описание";
    model->setHorizontalHeaderLabels(labels);
    ui->treeViewBranch->setModel(model);
    int row = 0;
    while (q.next())
    {
        model->blockSignals(true);
        model->insertRow(row);
        QModelIndex index = model->index(row, 0);
        model->setData(index, q.value(0),Qt::UserRole+1); //id
        model->setData(index, q.value(1)); //city_name
        index = model->index(row, 1);
        model->setData(index, q.value(2)); //city_id
        model->blockSignals(false);
        ui->comboBoxCurrentBranch->addItem(q.value(1).toString(),q.value(0));
        model->sort(1);
        ++row;
    }
}

void BranchWidget::onAddBranchClicked()
{
    QSqlQuery q;
    if (!getSqlQuery(q))        
        return;
    q.prepare("insert into Branch(branch_name,branch_description) values(?,?) returning id");
    q.addBindValue(trUtf8("Название"));
    q.addBindValue(trUtf8("Описание"));
    if (!q.exec())
        return;
    int row = model->rowCount();
    model->insertRow(row);
    model->blockSignals(true);
    QModelIndex index;
    index = model->index(row, 0);
    model->setData(index,"Название");
    if (!q.next())
        return;
    model->setData(index,q.value(0),Qt::UserRole + 1 );
    index = model->index(row,1);
    model->blockSignals(false);
    model->setData(index,"Описание");
}

void BranchWidget::onRemoveBranchClicked()
{
    QModelIndexList list = ui->treeViewBranch->selectionModel()->selectedRows();
    QModelIndex index;
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    for (int i = 0; i < list.size(); i++) {
        index = model->index(list.at(i).row(), 0);
        q.prepare("delete from Branch where id = ?");
        q.addBindValue(model->data(index,Qt::UserRole+1));
        q.exec();
        model->removeRow(list.at(i).row(),list.at(i).parent());
    }
    ui->treeViewBranch->selectionModel()->clearSelection();
}

void BranchWidget::onCurrentBrancheChanged()
{
    QSettings settings;
    settings.setValue("db/CurrentBranch",ui->comboBoxCurrentBranch->itemData(ui->comboBoxCurrentBranch->currentIndex()).toString());
}

void BranchWidget::onBranchViewChanged(QStandardItem *qsi)
{
    QSqlQuery q;
     if (!getSqlQuery(q))
         return;
     QModelIndex index = model->index(qsi->index().row(),0);
     q.prepare("UPDATE Branch SET branch_name = ?, branch_description = ? where ID = ?");
     q.addBindValue(model->data(index));
     index = model->index(qsi->index().row(),1);
     q.addBindValue(model->data(index));
     index = model->index(qsi->index().row(), 0);//id
     q.addBindValue(model->data(index,Qt::UserRole+1));
     q.exec();
}

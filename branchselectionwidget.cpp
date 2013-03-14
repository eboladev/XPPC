#include "branchselectionwidget.h"
#include "ui_branchselectionwidget.h"
#include "setupmanager.h"

#include <QSqlQuery>
#include <QSqlDatabase>
BranchSelectionWidget::BranchSelectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BranchSelectionWidget)
{
    ui->setupUi(this);
    QSqlQuery q;
    if (!SetupManager::instance()->getSqlQueryForDB(q))
        return;
    q.exec("select id, branch_name from branch");
    while(q.next())
        ui->comboBoxBranch->addItem(q.value(1).toString(),q.value(0));
    ui->comboBoxBranch->setCurrentIndex(ui->comboBoxBranch->findData(SetupManager::instance()->getCurrentBranch()));
}

BranchSelectionWidget::~BranchSelectionWidget()
{
    delete ui;
}

void BranchSelectionWidget::setBranch(QVariant id)
{
    ui->comboBoxBranch->setCurrentIndex(ui->comboBoxBranch->findData(id));
}

QVariant BranchSelectionWidget::getCurrentBranch()
{
    return ui->comboBoxBranch->itemData(ui->comboBoxBranch->currentIndex());
}

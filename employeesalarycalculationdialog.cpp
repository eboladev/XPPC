#include "employeesalarycalculationdialog.h"
#include "ui_employeesalarycalculationdialog.h"

#include "employeewidget.h"
#include "usersandpermissionsmanager.h"
#include "itemmodelforemployeepenaltyandbonuses.h"
#include "dialogtemplate.h"
#include "employeepenaltyandbonusesdialog.h"

#include <QDebug>

EmployeeSalaryCalculationDialog::EmployeeSalaryCalculationDialog(const QString &dbConnectionString, QWidget *parent) :
    QDialog(parent),
    SqlExtension(dbConnectionString),
    m_dbConnectionString(dbConnectionString),
    ui(new Ui::EmployeeSalaryCalculationDialog)
{
    ui->setupUi(this);
    ui->groupBoxWorkDays->setVisible(false);
    employeeWidget = new EmployeeWidget(dbConnectionString, ui->widgetEmployee);
    penaltyAndBonusesModel = new ItemModelForEmployeePenaltyAndBonuses(dbConnectionString,this);
    ui->widgetEmployee->setMinimumHeight(employeeWidget->geometry().height());
    ui->widgetEmployee->setMinimumWidth(employeeWidget->geometry().width());
    jobs = new JobItemModel(dbConnectionString,this);
    ui->treeViewEmployeeJobsList->setModel(jobs);
    connect(ui->pushButtonCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui->pushButtonOk, SIGNAL(clicked()), this, SLOT(accept()));
    initJobsTable();
    ui->groupBoxTotal->setEnabled(accessManager->isCanEditSalary());
    connect(employeeWidget, SIGNAL(currentIndexChanged(int)), SLOT(onSetCurrentEmployeeSalaryPayments(int)));
    employeeWidget->setEmployeeCurrentId();
    parseJobs();
    connect(ui->spinBoxRate, SIGNAL(valueChanged(int)), SLOT(onRecalculateSalary(int)));
    connect(ui->spinBoxPercent, SIGNAL(valueChanged(int)), SLOT(onRecalculateSalary(int)));
    connect(ui->spinBoxSalePercent, SIGNAL(valueChanged(int)), SLOT(onRecalculateSalary(int)));
    connect(ui->spinBoxSPday, SIGNAL(valueChanged(int)), SLOT(onRecalculateSalary(int)));
    connect(jobs, SIGNAL(itemChanged(QStandardItem*)), SLOT(onItemChanged(QStandardItem*)));
    connect(ui->pushButtonConfirmSalary, SIGNAL(clicked()), SLOT(onConfirmSalaryClicked()));
    connect(ui->lineEditPAB, SIGNAL(textChanged(QString)), SLOT(onPABChanged(QString)));
    connect(ui->pushButtonSPManagement, SIGNAL(clicked()), SLOT(onBonusAndPenaltyManagementClicked()));
    connect(ui->checkBoxOnlyClosedTickets, SIGNAL(toggled(bool)),
            SLOT(recalculateJobs(bool)));
}

EmployeeSalaryCalculationDialog::~EmployeeSalaryCalculationDialog()
{
    delete ui;
}

void EmployeeSalaryCalculationDialog::parseJobs()
{
    initJobsTable();
    getEmployeePenaltyAndBonuses();
    QHash<QString, QList<Job> > jobsHash = jobs->getEmployeeJobs(employeeWidget->getCurrentEmployeeId(),
                                                                 ui->checkBoxOnlyClosedTickets->checkState() == Qt::Checked);
    QHashIterator<QString, QList<Job> > it(jobsHash);
    while (it.hasNext())
    {
        it.next();
        QStandardItem* ticketItem = new QStandardItem(it.key());
        ticketItem->setEditable(false);
        foreach (Job job, it.value()) {
            QStandardItem* jobName = new QStandardItem(job.name);
            jobName->setData(job.id);
            jobName->setEditable(false);
            QStandardItem* jobQuant = new QStandardItem(QString::number(job.quantity));
            QStandardItem* jobPrice = new QStandardItem(QString::number(job.price));
            QStandardItem* jobDate = new QStandardItem(job.date.toString("dd-MM-yyyy hh:mm:ss"));
            jobDate->setEditable(false);
            QStandardItem* jobTotal = new QStandardItem(QString::number(job.quantity*job.price));
            jobTotal->setEditable(false);
            QStandardItem* salary = new QStandardItem(QString::number(job.quantity*job.price*(ui->spinBoxPercent->value()/100)));
            ticketItem->appendRow(QList<QStandardItem*>() << jobDate << jobName << jobQuant << jobPrice << jobTotal << salary );
        }
        QStandardItem* emptyItem = new QStandardItem();
        emptyItem->setEditable(false);
        QStandardItem* emptyItem2 = new QStandardItem();
        emptyItem2->setEditable(false);
        QStandardItem* emptyItem3 = new QStandardItem();
        emptyItem3->setEditable(false);
        QStandardItem* emptyItem4 = new QStandardItem();
        emptyItem4->setEditable(false);
        QStandardItem* emptyItem5 = new QStandardItem();
        emptyItem5->setEditable(false);
        jobsItem->appendRow(QList<QStandardItem*>()
                            << ticketItem
                            << emptyItem
                            << emptyItem2
                            << emptyItem3
                            << emptyItem4
                            << emptyItem5);
    }
    ui->treeViewEmployeeJobsList->expandAll();
    ui->treeViewEmployeeJobsList->resizeColumnToContents(0);
    ui->treeViewEmployeeJobsList->resizeColumnToContents(1);
    recalculateSalary();
}

void EmployeeSalaryCalculationDialog::calculateSummarySalary()
{
    float summary = 0;
    for(int ticketInd = 0 ; ticketInd < jobsItem->rowCount(); ++ticketInd)
    {
        for (int jobInd = 0 ; jobInd < jobsItem->child(ticketInd,0)->rowCount(); ++jobInd)
        {
            summary+= jobsItem->child(ticketInd,0)->child(jobInd,5)->text().toFloat();
        }
    }
    ui->lineEditTotal->setText(QString::number(summary+ui->lineEditPAB->text().toFloat()));
}

void EmployeeSalaryCalculationDialog::recalculateSalary()
{
    for(int ticketInd = 0 ; ticketInd < jobsItem->rowCount(); ++ticketInd)
    {
        for (int jobInd = 0 ; jobInd < jobsItem->child(ticketInd,0)->rowCount(); ++jobInd)
        {
            float salary = jobsItem->child(ticketInd,0)->child(jobInd,2)->text().toFloat()*
                    jobsItem->child(ticketInd,0)->child(jobInd,3)->text().toFloat()*
                    ((float)ui->spinBoxPercent->value()/100);
            jobsItem->child(ticketInd,0)->child(jobInd,5)->setText(QString::number(salary));
        }
    }
    calculateSummarySalary();
}

void EmployeeSalaryCalculationDialog::initJobsItem()
{
    jobsItem = new QStandardItem(trUtf8("Работы по квитанциям"));
    jobsItem->setEditable(false);
    QStandardItem* emptyItem = new QStandardItem();
    emptyItem->setEditable(false);
    QStandardItem* emptyItem2 = new QStandardItem();
    emptyItem2->setEditable(false);
    QStandardItem* emptyItem3 = new QStandardItem();
    emptyItem3->setEditable(false);
    QStandardItem* emptyItem4 = new QStandardItem();
    emptyItem4->setEditable(false);
    QStandardItem* emptyItem5 = new QStandardItem();
    emptyItem5->setEditable(false);
    jobs->appendRow(QList<QStandardItem*>()
                        << jobsItem
                        << emptyItem
                        << emptyItem2
                        << emptyItem3
                        << emptyItem4
                        << emptyItem5);
}

void EmployeeSalaryCalculationDialog::initSoldProductsItem()
{
    soldProductsItem = new QStandardItem(trUtf8("Продажи"));
    soldProductsItem->setEditable(false);
    QStandardItem* emptyItem = new QStandardItem();
    emptyItem->setEditable(false);
    QStandardItem* emptyItem2 = new QStandardItem();
    emptyItem2->setEditable(false);
    QStandardItem* emptyItem3 = new QStandardItem();
    emptyItem3->setEditable(false);
    QStandardItem* emptyItem4 = new QStandardItem();
    emptyItem4->setEditable(false);
    QStandardItem* emptyItem5 = new QStandardItem();
    emptyItem5->setEditable(false);
    jobs->appendRow(QList<QStandardItem*>()
                        << soldProductsItem
                        << emptyItem
                        << emptyItem2
                        << emptyItem3
                        << emptyItem4
                    << emptyItem5);
}

void EmployeeSalaryCalculationDialog::initJobsTable()
{
    jobs->clear();
    jobs->setHorizontalHeaderLabels(QStringList() << trUtf8("Тип\n№\nДата")
                                    << trUtf8("Наименование")
                                    << trUtf8("Количество")
                                    << trUtf8("Цена")
                                    << trUtf8("Сумма")
                                    << trUtf8("З\п"));
    jobs->setColumnCount(6);
    initJobsItem();
    initSoldProductsItem();
}

void EmployeeSalaryCalculationDialog::getEmployeePenaltyAndBonuses()
{
    penaltyAndBonusesModel->getPendingPenaltyAndBonuses(employeeWidget->getCurrentEmployeeId().toString());
    ui->lineEditPAB->setText(QString::number(penaltyAndBonusesModel->getTotalAmount()));
}

void EmployeeSalaryCalculationDialog::onSetCurrentEmployeeSalaryPayments(int)
{
    parseJobs();
    getEmployeePenaltyAndBonuses();
    ui->spinBoxRate->blockSignals(true);
    ui->spinBoxRate->setValue(employeeWidget->getCurrentEmployeeRate());
    ui->spinBoxRate->blockSignals(false);
    ui->spinBoxPercent->blockSignals(true);
    ui->spinBoxPercent->setValue(employeeWidget->getCurrentEmployeeJobPercent());
    ui->spinBoxPercent->blockSignals(false);
    ui->spinBoxSalePercent->blockSignals(true);
    ui->spinBoxSalePercent->setValue(employeeWidget->getCurrentEmployeeSalePercent());
    ui->spinBoxSalePercent->blockSignals(false);
    ui->spinBoxSPday->blockSignals(true);
    ui->spinBoxSPday->setValue(employeeWidget->getCurrentEmployeeSalaryPerDay());
    ui->spinBoxSPday->blockSignals(false);
    recalculateSalary();
}

void EmployeeSalaryCalculationDialog::onRecalculateSalary(int)
{
    recalculateSalary();
}

void EmployeeSalaryCalculationDialog::onItemChanged(QStandardItem * changedItem)
{
    int column = changedItem->column();
    if ((column == 2) || (column == 3))
        recalculateSalary();
    if (column == 5)
        calculateSummarySalary();
}

void EmployeeSalaryCalculationDialog::onConfirmSalaryClicked()
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    for(int ticketInd = 0 ; ticketInd < jobsItem->rowCount(); ++ticketInd)
    {
        for (int jobInd = 0 ; jobInd < jobsItem->child(ticketInd,0)->rowCount(); ++jobInd)
        {
            q.prepare("update JobOnTicket set job_payed_datetime = ? where jot_id = ?");
            q.addBindValue(QDateTime::currentDateTime());
            q.addBindValue(jobsItem->child(ticketInd,0)->child(jobInd,1)->data());
            if (!q.exec())
                qDebug() << q.lastQuery() << q.lastError() << Q_FUNC_INFO;
        }
    }
    q.prepare("update employee_penalty_and_bonuses set inure_date = ? where employee_id = ?");
    q.addBindValue(QDateTime::currentDateTime());
    q.addBindValue(employeeWidget->getCurrentEmployeeId());
    if (!q.exec())
        qDebug() << q.lastQuery() << q.lastError();
    parseJobs();
}

void EmployeeSalaryCalculationDialog::onBonusAndPenaltyManagementClicked()
{
    if (DialogTemplate::executeDialogWithDbCheck<EmployeePenaltyAndBonusesDialog>(this))
    {
        getEmployeePenaltyAndBonuses();
        calculateSummarySalary();
    }
}

void EmployeeSalaryCalculationDialog::onPABChanged(QString)
{
    calculateSummarySalary();
}

void EmployeeSalaryCalculationDialog::recalculateJobs(const bool &state)
{
    Q_UNUSED(state);
    parseJobs();
}

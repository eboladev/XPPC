#include "employeesalarycalculationdialog.h"
#include "ui_employeesalarycalculationdialog.h"

#include "employeewidget.h"
#include "usersandpermissionsmanager.h"

#include <QDebug>

EmployeeSalaryCalculationDialog::EmployeeSalaryCalculationDialog(const QString &dbConnectionString, QWidget *parent) :
    QDialog(parent),
    SqlExtension(dbConnectionString),
    ui(new Ui::EmployeeSalaryCalculationDialog)
{
    ui->setupUi(this);
    employeeWidget = new EmployeeWidget(dbConnectionString, ui->widgetEmployee);
    employeeWidget->setEmployeeCurrentId();
    adjustSize();
    ui->widgetEmployee->setMinimumHeight(employeeWidget->geometry().height());
    ui->widgetEmployee->setMinimumWidth(employeeWidget->geometry().width());

    jobs = new JobItemModel(dbConnectionString,this);
    ui->treeViewEmployeeJobsList->setModel(jobs);
    //connect(ui->comboBoxEmployee, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentEmployeeChanged(int)));
    connect(ui->pushButtonCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui->pushButtonOk, SIGNAL(clicked()), this, SLOT(accept()));
    jobsItem = new QStandardItem(trUtf8("Работы по квитанциям"));
    soldProductsItem = new QStandardItem(trUtf8("Проданные товары"));
    jobs->setColumnCount(6);
    jobs->appendRow(jobsItem);
    jobs->appendRow(soldProductsItem);
    ui->groupBoxTotal->setEnabled(accessManager->isCanEditSalary());
}

EmployeeSalaryCalculationDialog::~EmployeeSalaryCalculationDialog()
{
    delete ui;
}

void EmployeeSalaryCalculationDialog::onCurrentEmployeeChanged(const int &current)
{
    //ui->lineEditTotal->setText(QString::number(jobs->getEmployeeJobs(ui->comboBoxEmployee->itemData(current,IDrole),true)));
    //parseJobs(jobs->getEmployeeJobs(ui->comboBoxEmployee->itemData(current,IDrole)));
    //ui->treeViewEmployeeJobsList->resizeColumnsToContents();
    ui->treeViewEmployeeJobsList->expandAll();
    ui->treeViewEmployeeJobsList->resizeColumnToContents(0);
    ui->treeViewEmployeeJobsList->resizeColumnToContents(1);
}

void EmployeeSalaryCalculationDialog::parseJobs(QHash<QString, QList<Job> > jobsHash)
{
    QHashIterator<QString, QList<Job> > it(jobsHash);
    while (it.hasNext())
    {
        it.next();
        QStandardItem* ticketItem = new QStandardItem(it.key());
        foreach (Job job, it.value()) {
            QStandardItem* jobName = new QStandardItem(job.name);
            QStandardItem* jobQuant = new QStandardItem(QString::number(job.quantity));
            QStandardItem* jobPrice = new QStandardItem(QString::number(job.price));
            QStandardItem* jobDate = new QStandardItem(job.date.toString("dd-MM-yyyy hh:mm:ss"));
            QStandardItem* jobTotal = new QStandardItem(QString::number(job.quantity*job.price));
            QStandardItem* salary = new QStandardItem("0");
            ticketItem->appendRow(QList<QStandardItem*>() << jobDate << jobName << jobQuant << jobPrice << jobTotal << salary );
        }
        jobsItem->appendRow(QList<QStandardItem*>() << ticketItem << new QStandardItem()<< new QStandardItem()<< new QStandardItem()<< new QStandardItem()<< new QStandardItem());
    }
}

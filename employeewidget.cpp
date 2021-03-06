#include "employeewidget.h"
#include "ui_employeewidget.h"

#include "setupmanager.h"
#include "employeeitemmodel.h"
#include "usersandpermissionsmanager.h"

#include <QSortFilterProxyModel>
#include <QDebug>

EmployeeWidget::EmployeeWidget(const QString &dbConnectionString, QWidget *parent) :
    QWidget(parent),
    SqlExtension(dbConnectionString),
    ui(new Ui::EmployeeWidget)
{
    ui->setupUi(this);
    employeeModel = new EmployeeItemModel(dbConnectionString, true, this);
    employeeProxyModel = new QSortFilterProxyModel(this);
    employeeProxyModel->setSourceModel(employeeModel);
    employeeProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->comboBoxEmployee->setModel(employeeProxyModel);
    setMinimumWidth(parent->minimumWidth());
    connect(ui->lineEditEmployeeFilter, SIGNAL(textChanged(QString)), employeeProxyModel, SLOT(setFilterFixedString(QString)));
    connect(ui->comboBoxEmployee, SIGNAL(currentIndexChanged(int)), SIGNAL(currentIndexChanged(int)));
    setEnabled(accessManager->isCanEditJobList());
}

EmployeeWidget::~EmployeeWidget()
{
    delete ui;
}

QVariant EmployeeWidget::getCurrentEmployeeId() const
{
    return ui->comboBoxEmployee->itemData(ui->comboBoxEmployee->currentIndex(), IDrole);
}

void EmployeeWidget::setEmployeeCurrentId(const int &id)
{
    ui->comboBoxEmployee->setCurrentIndex(id);
}

void EmployeeWidget::setEmployeeCurrentId()
{
    ui->comboBoxEmployee->setCurrentIndex(ui->comboBoxEmployee->findData(employeeModel->getCurrentId(),Qt::UserRole + 1));
}

QString EmployeeWidget::getCurrentEmployeeName()
{
    return ui->comboBoxEmployee->currentText();
}

float EmployeeWidget::getCurrentEmployeeRate() const
{
    return employeeModel->getEmployeeRate(getCurrentEmployeeId());
}

float EmployeeWidget::getCurrentEmployeeJobPercent() const
{
    return employeeModel->getEmployeeJobPercent(getCurrentEmployeeId());
}

float EmployeeWidget::getCurrentEmployeeSalePercent() const
{
    return employeeModel->getEmployeeSalePercent(getCurrentEmployeeId());
}

float EmployeeWidget::getCurrentEmployeeSalaryPerDay() const
{
    return employeeModel->getEmployeeSalaryPerDay(getCurrentEmployeeId());
}

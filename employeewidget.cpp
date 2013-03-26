#include "employeewidget.h"
#include "ui_employeewidget.h"

#include "setupmanager.h"
#include "employeeitemmodel.h"

#include <QSortFilterProxyModel>

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
}

EmployeeWidget::~EmployeeWidget()
{
    delete ui;
}

QVariant EmployeeWidget::getCurrentEmployeeId() const
{
    return ui->comboBoxEmployee->itemData(ui->comboBoxEmployee->currentIndex());
}

void EmployeeWidget::setEmployeeCurrentId(const int &id)
{
    ui->comboBoxEmployee->setCurrentIndex(id);
}

void EmployeeWidget::setEmployeeCurrentId()
{
    ui->comboBoxEmployee->setCurrentIndex(ui->comboBoxEmployee->findData(employeeModel->getCurrentId(),Qt::UserRole + 1));
}

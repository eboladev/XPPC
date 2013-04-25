#include "employeepabdialog.h"
#include "ui_employeepabdialog.h"

EmployeePABDialog::EmployeePABDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmployeePABDialog)
{
    ui->setupUi(this);
    connect(ui->pushButtonOk, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->pushButtonCancel, SIGNAL(clicked()), SLOT(reject()));
}

EmployeePABDialog::~EmployeePABDialog()
{
    delete ui;
}

int EmployeePABDialog::getAmount()
{
    return ui->spinBoxAmount->value();
}

QString EmployeePABDialog::getReason()
{
    return ui->plainTextEditReason->toPlainText().trimmed();
}

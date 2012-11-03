#include "joblistonreceiptdialog.h"
#include "ui_joblistonreceiptdialog.h"

JobListOnReceiptDialog::JobListOnReceiptDialog(const QString dbConnectionsString, const int id, QWidget *parent) :
    QDialog(parent),
    SqlExtension(dbConnectionsString),
    ui(new Ui::JobListOnReceiptDialog)
{
    ui->setupUi(this);
    connect(ui->pushButtonOK, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->pushButtonCancel,SIGNAL(clicked()),this, SLOT(reject()));
    getEmployeeList();
}

JobListOnReceiptDialog::~JobListOnReceiptDialog()
{
    delete ui;
}

void JobListOnReceiptDialog::getEmployeeList()
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    QStringList sl;
    q.exec("select employee_fio,employee_id from Employee");
    while (q.next())
        ui->comboBoxEmployeeList->addItem(q.value(0).toString(),q.value(1).toInt());
//        sl.append(q.value(0).toString());

//    ui->comboBoxEmployeeList->addItems(sl);
}

void JobListOnReceiptDialog::on_comboBoxEmployeeList_currentIndexChanged(int index)
{
    ui->lineEditQuantity->setText(ui->comboBoxEmployeeList->itemData(index).toString());
}

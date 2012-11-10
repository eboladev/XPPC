#include "joblistonreceiptdialog.h"
#include "ui_joblistonreceiptdialog.h"

JobListOnReceiptDialog::JobListOnReceiptDialog(const QString dbConnectionsString, const int &id, QWidget *parent) :
    QDialog(parent),
    SqlExtension(dbConnectionsString),
    m_id(id),
    ui(new Ui::JobListOnReceiptDialog)
{
    ui->setupUi(this);
    model = new QSqlQueryModel(this);
    connect(ui->pushButtonOK, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->pushButtonCancel,SIGNAL(clicked()),this, SLOT(reject()));
    getEmployeeList();

    if (id!=0)
        getJobs(id);

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
}

void JobListOnReceiptDialog::getJobs(const int &id)
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.prepare("select employee_FIO,job_name,job_quantity,job_price,Job_date,jot_id from JobOnTicket join Employee ON (JobOnTicket.Employee_ID=Employee.Employee_ID) where Ticket_ID=?");
    q.addBindValue(id);
    if (!q.exec())
        return;

    model->setQuery(q);
    model->query().exec();

    model->setHeaderData(0, Qt::Horizontal, tr("Мастер"));            //0
    model->setHeaderData(1, Qt::Horizontal, tr("Наименование"));          //1
    model->setHeaderData(2, Qt::Horizontal, tr("Количество"));      //2
    model->setHeaderData(3, Qt::Horizontal, tr("Цена"));   //3
    model->setHeaderData(4, Qt::Horizontal, tr("Дата"));    //4
    model->setHeaderData(5,Qt::Horizontal,tr("ИД"));
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setColumnHidden(5,true);
//    ui->tableViewTicket->resizeColumnToContents(2);
//    ui->tableViewTicket->setColumnWidth(1,70);
//    ui->tableViewTicket->setColumnWidth(3,120);
//    ui->tableViewTicket->setColumnWidth(4,200);
}

void JobListOnReceiptDialog::on_comboBoxEmployeeList_currentIndexChanged(int index)
{
    ui->lineEditQuantity->setText(ui->comboBoxEmployeeList->itemData(index).toString());
}

void JobListOnReceiptDialog::on_pushButtonClearField_clicked()
{
    clearField();
}

void JobListOnReceiptDialog::clearField()
{
    ui->lineEditJobName->clear();
    ui->lineEditPrice->clear();
    ui->lineEditQuantity->setText("1");
    ui->comboBoxEmployeeList->setCurrentIndex(0);
}

void JobListOnReceiptDialog::on_pushButtonAddJob_clicked()
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.prepare("insert into JobOnTicket(ticket_id,employee_id,job_name,job_quantity,job_date,job_price) values(?,?,?,?,(select currentdate from getcurrentdate),?)");
    q.addBindValue(m_id);
    q.addBindValue(ui->comboBoxEmployeeList->itemData(ui->comboBoxEmployeeList->currentIndex()));
    q.addBindValue(ui->lineEditJobName->text().toUtf8());
    q.addBindValue(ui->lineEditQuantity->text().toUtf8());
    q.addBindValue(ui->lineEditPrice->text().toUtf8());
    if (!q.exec())
       return;
    getJobs(m_id);
}

void JobListOnReceiptDialog::on_pushButtonDeleteJob_clicked()
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.prepare("delete from JobOnTicket where jot_id=?");
    q.addBindValue(model->record(ui->tableView->currentIndex().row()).value(5));
    if (!q.exec())
        qDebug() << q.lastError();
    getJobs(m_id);
}

void JobListOnReceiptDialog::on_tableView_clicked(const QModelIndex &index)
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.prepare("select JobOnTicket.employee_id,job_name,job_quantity,job_price from JobOnTicket join Employee ON (JobOnTicket.Employee_ID=Employee.Employee_ID) where jot_id=?");
    q.addBindValue(model->record(index.row()).value(5).toInt());
    if (!q.exec())
    {
        qDebug() << q.lastError();
        return;
    }
    while (q.next())
    {

        ui->comboBoxEmployeeList->setCurrentIndex(ui->comboBoxEmployeeList->findData(q.value(0)));
        //ui->comboBoxEmployeeList->findText(q.value(0).toString());
        ui->lineEditJobName->setText(q.value(1).toString());
        ui->lineEditPrice->setText(q.value(3).toString());
        ui->lineEditQuantity->setText(q.value(2).toString());
        m_jid = model->record(index.row()).value(5).toInt();
     }
}

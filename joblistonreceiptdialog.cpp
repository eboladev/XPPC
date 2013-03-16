#include "joblistonreceiptdialog.h"
#include "ui_joblistonreceiptdialog.h"
#include "setupmanager.h"

#include <QStandardItemModel>

JobListOnReceiptDialog::JobListOnReceiptDialog(const QString dbConnectionsString, const int &id, QWidget *parent) :
    QDialog(parent),
    SqlExtension(dbConnectionsString),
    m_id(id),
    ui(new Ui::JobListOnReceiptDialog)
{
    ui->setupUi(this);

    model = new QStandardItemModel(this);
    ui->tableView->setModel(model);
    setWindowTitle(trUtf8("Управление списком работ по квитанции №").append(QString::number(id)));
    connect(ui->pushButtonOK, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->pushButtonCancel,SIGNAL(clicked()),this, SLOT(reject()));
    connect(ui->pushButtonCheckReady,SIGNAL(clicked()),this,SLOT(on_pushButtonCheckReady_clicked()));
    connect(ui->pushButtonUpdateJob, SIGNAL(clicked()), this, SLOT(onUpdateClicked()));
    connect(ui->tableView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(onCurrentSelectionChanged(QModelIndex,QModelIndex)));
    getEmployeeList();
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

    if (!q.exec("select employee_fio, employee_id, login from Employee"))
        return;
    int row = 0;
    int index = 0;
    while (q.next())
    {
        if (SetupManager::instance()->getCurrentUser().compare(q.value(2).toString()) == 0)
            index = row;
        ui->comboBoxEmployeeList->addItem(q.value(0).toString(),q.value(1).toInt());
        ++row;
    }
    ui->comboBoxEmployeeList->setCurrentIndex(index);
}

void JobListOnReceiptDialog::getJobs(const int &id)
{
    model->clear();
    model->setHorizontalHeaderLabels(QStringList() << trUtf8("Мастер") << trUtf8("Наименование")
                                     << trUtf8("Количество") << trUtf8("Цена") << trUtf8("Дата"));
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;

    q.prepare("select employee_FIO,job_name,job_quantity,job_price,Job_date,jot_id from JobOnTicket "
              "join Employee ON (JobOnTicket.Employee_ID=Employee.Employee_ID) where tdc_r_id=?");
    q.addBindValue(id);
    if (!q.exec())
        return;

    while (q.next())
    {
        QStandardItem* fio = new QStandardItem(q.value(0).toString());
        fio->setData(q.value(5));
        QStandardItem* jobName = new QStandardItem(q.value(1).toString());
        QStandardItem* jobQuant = new QStandardItem(q.value(2).toString());
        QStandardItem* jobPrice = new QStandardItem(q.value(3).toString());
        QStandardItem* jobDate = new QStandardItem(q.value(4).toString());
        model->appendRow(QList<QStandardItem*>() << fio << jobName << jobQuant << jobPrice << jobDate);
    }
    ui->tableView->resizeColumnsToContents();    
}

void JobListOnReceiptDialog::on_pushButtonClearField_clicked()
{
    clearField();
}

void JobListOnReceiptDialog::clearField()
{
    ui->lineEditJobName->clear();
    ui->lineEditPrice->clear();
    ui->spinBoxQuantity->setValue(1);    
}

void JobListOnReceiptDialog::on_pushButtonAddJob_clicked()
{
    if (ui->lineEditJobName->text().isEmpty() ||
            ui->lineEditPrice->text().isEmpty())
        return;
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.prepare("insert into JobOnTicket(tdc_r_id,employee_id,job_name,job_quantity,job_price) values(?,?,?,?,?)");
    q.addBindValue(m_id);
    q.addBindValue(ui->comboBoxEmployeeList->itemData(ui->comboBoxEmployeeList->currentIndex()));
    q.addBindValue(ui->lineEditJobName->text());
    q.addBindValue(ui->spinBoxQuantity->value());
    q.addBindValue(ui->lineEditPrice->text().toInt());
    if (!q.exec())
    {
        qDebug() << q.lastError() << q.lastQuery();
        return;
    }
    getJobs(m_id);
}

void JobListOnReceiptDialog::on_pushButtonDeleteJob_clicked()
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.prepare("delete from JobOnTicket where jot_id=?");
    q.addBindValue(model->item(ui->tableView->currentIndex().row(),0)->data());
    if (!q.exec())
        return;
    getJobs(m_id);
}

void JobListOnReceiptDialog::onUpdateClicked()
{
    if (!ui->tableView->currentIndex().isValid())
        return;
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.prepare("update JobOnTicket set employee_id = ?, job_name = ?, "
              "job_quantity = ?, job_price = ? where jot_id = ?");
    q.addBindValue(ui->comboBoxEmployeeList->itemData(ui->comboBoxEmployeeList->currentIndex()));
    q.addBindValue(ui->lineEditJobName->text());
    q.addBindValue(ui->spinBoxQuantity->value());
    q.addBindValue(ui->lineEditPrice->text());
    q.addBindValue(model->item(ui->tableView->currentIndex().row(),0)->data());
    q.exec();
    getJobs(m_id);
}

void JobListOnReceiptDialog::onCurrentSelectionChanged(QModelIndex current, QModelIndex previous)
{
    Q_UNUSED(previous);
    if (!current.isValid())
        return;
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.prepare("select JobOnTicket.employee_id,job_name,job_quantity,job_price from JobOnTicket "
              "join Employee ON (JobOnTicket.Employee_ID=Employee.Employee_ID) where jot_id=?");
    q.addBindValue(model->item(current.row(),0)->data());
    if (!q.exec())
        return;
    while (q.next())
    {
        ui->comboBoxEmployeeList->setCurrentIndex(ui->comboBoxEmployeeList->findData(q.value(0)));
        ui->lineEditJobName->setText(q.value(1).toString());
        ui->lineEditPrice->setText(q.value(3).toString());
        ui->spinBoxQuantity->setValue(q.value(2).toInt());
        m_jid = model->item(ui->tableView->currentIndex().row(),0)->data().toInt();
    }
}

void JobListOnReceiptDialog::on_pushButtonCheckReady_clicked()
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.prepare("update ticket set status = 0 whereid = "
              "(select distinct ticket_id from tdc_relation where id = ?)");
    q.addBindValue(m_id);
    if (!q.exec())
    {
        qDebug() << q.lastError() << q.lastQuery();
        return;
    }

    accept();
}

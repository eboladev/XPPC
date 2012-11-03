#include "receiptmanager.h"
#include "ui_receiptmanager.h"

ReceiptManager::ReceiptManager(const QString dbConnectionsString, const int id, QWidget *parent) :
    QDialog(parent),    
    SqlExtension(dbConnectionsString),
    ui(new Ui::ReceiptManager)
{
ui->setupUi(this);
clearFields();
fillFields(id);
connect(ui->pushButtonAddReceipt, SIGNAL(clicked()), this, SLOT(accept()));
connect(ui->pushButtonClose,SIGNAL(clicked()),this, SLOT(reject()));
}

ReceiptManager::ReceiptManager(const QString dbConnectionsString, QWidget *parent)
{
    ui->setupUi(this);
    clearFields();
    connect(ui->pushButtonAddReceipt, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->pushButtonClose,SIGNAL(clicked()),this, SLOT(reject()));
}

ReceiptManager::~ReceiptManager()
{
    delete ui;
}



void ReceiptManager::on_pushButtonAddReceipt_clicked()
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.exec("insert into Ticket(Ticket_FIO,Ticket_phone,Ticket_device,Ticket_Serial,Ticket_qual,Ticket_problem,Ticket_date_in) VALUES('"+
           ui->lineEditFIO->text()+"','"+ui->lineEditPhone->text()+"','"+ui->lineEditDevice->text()+"','"+ui->lineEditSerial->text()+
           "','"+ui->lineEditQual->text()+"','"+ui->plainTextEditMalfunction->toPlainText()+"', CURDATE())");
//    qDebug() << q.lastError();
    if (!q.next())
        return;
//    qDebug() << "hz";
//    if (!q.next())
//        return;
//    q.next();

//    qDebug() << q.value(0).toInt();
    // q.prepare("select ID from addgismeteomonitor(?, ?)");
}

void ReceiptManager::clearFields()
{
    ui->lineEditDevice->clear();
    ui->lineEditFIO->clear();
    ui->lineEditPhone->clear();
    ui->lineEditQual->clear();
    ui->lineEditSerial->clear();
    ui->plainTextEditMalfunction->clear();
}

void ReceiptManager::fillFields(int id)
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.exec("select ticket_FIO,ticket_phone,ticket_device,ticket_serial,ticket_qual,ticket_problem from Ticket where ticket_id="+QString::number(id));
//    qDebug() << q.lastError();
    if (!q.next())
        return;
    ui->lineEditDevice->setText(q.value(2).toString());
    ui->lineEditFIO->setText(q.value(0).toString());
    ui->lineEditPhone->setText(q.value(1).toString());
    ui->lineEditQual->setText(q.value(4).toString());
    ui->lineEditSerial->setText(q.value(3).toString());
    ui->plainTextEditMalfunction->setPlainText(q.value(5).toString());
//    qDebug() << "hz";
}

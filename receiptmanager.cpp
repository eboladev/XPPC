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
    ui->pushButtonAddReceipt->setText("Обновить квитанцию");
    ui->pushButtonAddReceipt->setProperty("mode",id);
    setWindowTitle("Управление квитанцией №"+id);
    connect(ui->pushButtonAddReceipt, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->pushButtonClose,SIGNAL(clicked()),this, SLOT(reject()));
}

ReceiptManager::ReceiptManager(const QString dbConnectionsString, QWidget *parent) :
    QDialog(parent),
    SqlExtension(dbConnectionsString),
    ui(new Ui::ReceiptManager)
{
    ui->setupUi(this);
    clearFields();
    QSettings settings;
    branch = settings.value("db/CurrentBranch").toInt();
    ui->pushButtonAddReceipt->setProperty("mode",0);
    setWindowTitle("Добавление квитанции");
    ui->pushButtonAddReceipt->setText("Добавить квитанцию");
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
    if (ui->pushButtonAddReceipt->property("mode").toInt() == 0)
    {
        q.exec("insert into Ticket(Ticket_FIO,Ticket_phone,Ticket_device,Ticket_Serial,Ticket_qual,Ticket_problem,Ticket_date_in,ticket_status,ticket_branch) VALUES('"+
               ui->lineEditFIO->text()+"','"+ui->lineEditPhone->text()+"','"+ui->lineEditDevice->text()+"','"+ui->lineEditSerial->text()+
               "','"+ui->lineEditQual->text()+"','"+ui->plainTextEditMalfunction->toPlainText()+"', (select currentdate from getcurrentdate),1,"+QString::number(branch)+")");
        if (!q.next())
            return;
    }
    else
    {
        q.prepare("update Ticket set ticket_fio = ? , ticket_phone = ?, ticket_device=?,ticket_serial=?,ticket_qual=?,ticket_problem=?,ticket_branch=? where ticket_id=?");
        q.addBindValue(ui->lineEditFIO->text().toUtf8());
        q.addBindValue(ui->lineEditPhone->text().toUtf8());
        q.addBindValue(ui->lineEditDevice->text().toUtf8());
        q.addBindValue(ui->lineEditSerial->text().toUtf8());
        q.addBindValue(ui->lineEditQual->text().toUtf8());
        q.addBindValue(ui->plainTextEditMalfunction->toPlainText().toUtf8());
        q.addBindValue(branch);
        q.addBindValue(ui->pushButtonAddReceipt->property("mode").toInt());
        if (!q.exec())
            return;
    }
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
    q.exec("select ticket_FIO,ticket_phone,ticket_device,ticket_serial,ticket_qual,ticket_problem,ticket_branch from Ticket where ticket_id="+QString::number(id));
    if (!q.next())
        return;
    ui->lineEditDevice->setText(q.value(2).toString());
    ui->lineEditFIO->setText(q.value(0).toString());
    ui->lineEditPhone->setText(q.value(1).toString());
    ui->lineEditQual->setText(q.value(4).toString());
    ui->lineEditSerial->setText(q.value(3).toString());
    ui->plainTextEditMalfunction->setPlainText(q.value(5).toString());
    branch = q.value(6).toInt();
}

void ReceiptManager::on_pushButtonClearFields_clicked()
{
    clearFields();
}

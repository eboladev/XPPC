#include "receiptmanager.h"
#include "ui_receiptmanager.h"
#include <QCompleter>
#include <QStringListModel>
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
    setWindowTitle(trUtf8("Управление квитанцией №").append(QString::number(id)));
    setupConnects();
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
    setupConnects();
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
        q.exec("insert into ticket(Ticket_FIO,Ticket_phone,Ticket_device,Ticket_Serial,Ticket_qual,Ticket_problem,Ticket_date_in,ticket_status,ticket_branch) VALUES('"+
               ui->lineEditFIO->text().trimmed()+"','"+ui->lineEditPhone->text().trimmed()+"','"+ui->lineEditDevice->text().trimmed()+"','"+ui->lineEditSerial->text().trimmed()+
               "','"+ui->lineEditQual->text().trimmed()+"','"+ui->plainTextEditMalfunction->toPlainText().trimmed()+"', (SELECT CURRENT_DATE),1,"+QString::number(branch)+")");
    }
    else
    {
        q.prepare("update Ticket set ticket_fio = ? , ticket_phone = ?, ticket_device=?,ticket_serial=?,ticket_qual=?,ticket_problem=?,ticket_branch=? where ticket_id=?");
        q.addBindValue(ui->lineEditFIO->text().trimmed());
        q.addBindValue(ui->lineEditPhone->text().trimmed());
        q.addBindValue(ui->lineEditDevice->text().trimmed());
        q.addBindValue(ui->lineEditSerial->text().trimmed());
        q.addBindValue(ui->lineEditQual->text().trimmed());
        q.addBindValue(ui->plainTextEditMalfunction->toPlainText().trimmed());
        q.addBindValue(branch);
        q.addBindValue(ui->pushButtonAddReceipt->property("mode").toInt());
        q.exec();
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

void ReceiptManager::onFIOTextChanged(QString FIO)
{
    if (FIO.length() > 4)
    {
        wordList.clear();
        model->removeRows(0,model->rowCount());
        QSqlQuery q;
        if (!getSqlQuery(q))
            return;
        q.exec("select ticket_fio from ticket where ticket_fio LIKE '%"+FIO+"%'");
        while(q.next())        
            wordList.append(q.value(0).toString());        
        model->setStringList(wordList);
        ui->lineEditFIO->completer()->popup();        
    }
}

void ReceiptManager::setupConnects()
{
    model = new QStringListModel(this);
    QCompleter *completer = new QCompleter(model, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    ui->lineEditFIO->setCompleter(completer);
    connect(ui->pushButtonAddReceipt, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->pushButtonClose,SIGNAL(clicked()),this, SLOT(reject()));
    connect(ui->lineEditFIO, SIGNAL(textEdited(QString)), this, SLOT(onFIOTextChanged(QString)));
}

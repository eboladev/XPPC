#include "closeticketwidget.h"
#include "ui_closeticketwidget.h"
#include <QSqlQueryModel>
CloseTicketWidget::CloseTicketWidget(const QString &dbConnectionName, const int &id, QWidget *parent) :
    QDialog(parent),
    m_id(id),
    SqlExtension(dbConnectionName),
    ui(new Ui::CloseTicketWidget)
{
    ui->setupUi(this);
    model = new QSqlQueryModel(this);

    if (id>0)
        getJobs(id);

    formPrice();

    connect(ui->pushButtonOk,SIGNAL(clicked()),this,SLOT(closeTicket()));
    connect(ui->pushButtonOk,SIGNAL(clicked()),this,SLOT(accept()));
    connect(ui->pushButtonClose,SIGNAL(clicked()),this,SLOT(reject()));
}

CloseTicketWidget::~CloseTicketWidget()
{
    delete ui;
}

void CloseTicketWidget::getJobs(const int &id)
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.prepare("select employee_FIO,job_name,job_quantity,job_price,Job_date,jot_id from JobOnTicket join Employee ON (JobOnTicket.Employee_ID=Employee.Employee_ID) where Ticket_ID=?");
    q.addBindValue(m_id);
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
    ui->treeViewJobsOnTIcket->setModel(model);
    ui->treeViewJobsOnTIcket->setColumnHidden(5,true);
}

void CloseTicketWidget::formPrice()
{
    int price = 0;
    for(int i = 0 ; i < model->rowCount(); ++i)
    {
        price += model->data(model->index(i,3)).toInt();
    }
    ui->lineEditPrice->setText(QString::number(price));
}

void CloseTicketWidget::closeTicket()
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.prepare("update ticket set ticket_date_out = (select currentdate from getcurrentdate), ticket_status = 2, ticket_price = ? where Ticket_ID=?");
    q.addBindValue(ui->lineEditPrice->text().toInt());
    q.addBindValue(m_id);
    q.exec();
    qDebug() << q.lastError();
//    if (!q.exec())
//        return;
}

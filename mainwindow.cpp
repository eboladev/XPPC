#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "receiptmanager.h"
#include "joblistonreceiptdialog.h"
#include "connectdialog.h"
#include "setupmanager.h"
#include "branchwidget.h"
#include "closeticketwidget.h"

#include "ncreport/include/ncreport.h"
#include "ncreport/include/ncreportoutput.h"
#include "ncreport/include/ncreportpreviewoutput.h"
#include "ncreport/include/ncreportpreviewwindow.h"

#include <QNetworkConfigurationManager>
#include <QNetworkConfiguration>
#include <QSqlQueryModel>

const QString CONNECTIONNAME = "XP";
const int DEFAULTPERIOD = 5000;
const int LIMIT = 1000;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new QSqlQueryModel(ui->tableViewTicket);

    QNetworkConfigurationManager* qnam= new QNetworkConfigurationManager();

    defaultConfName =  qnam->defaultConfiguration().name();
    connect(qnam,SIGNAL(configurationChanged(QNetworkConfiguration)),this,SLOT(networkFuckedUpTwo(QNetworkConfiguration)));

    model = new QSqlQueryModel(ui->tableViewTicket);
    updateTableViewTicket = new QTimer(this);
    currentStatus = InWork;
    connect(ui->actionOnAddReceiptClicked, SIGNAL(triggered()), this, SLOT(onAddReceiptClicked()));
    connect(ui->actionOnJobListClicked,SIGNAL(triggered()),this,SLOT(onJobListClicked()));
    connect(ui->actionExitMenuClicked,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionSettingsMenuClicked,SIGNAL(triggered()),this,SLOT(onSettingsClicked()));
    connect(ui->actionBranchTriggered,SIGNAL(triggered()),this,SLOT(on_actionBranches_triggered()));
    connect(ui->actionCloseTicket,SIGNAL(triggered()),this,SLOT(on_actionCloseTicket_triggered()));
    if (!checkDbSettings())
    {
        ui->actionConnect->setEnabled(false);
    }

    on_actionConnect_triggered();

    connect(updateTableViewTicket,SIGNAL(timeout()),this,SLOT(makeUpdate()));
    updateTableViewTicket->start(DEFAULTPERIOD);
}

MainWindow::~MainWindow()
{
    delete updateTableViewTicket;
    model->clear();
    delete model;
    QStringList connames = QSqlDatabase::connectionNames();
    for (int i = 0 ; i < connames.count(); ++i)
    {
        QSqlDatabase::database(connames.value(i)).close();
        QSqlDatabase::removeDatabase(connames.value(i));
    }
    delete ui;
}

void MainWindow::networkFuckedUpTwo(const QNetworkConfiguration &qnc)
{
    if ((qnc.state() != QNetworkConfiguration::Active) && (qnc.name() == defaultConfName))
    {
        qDebug() << qnc.name() << " some problems detected";
        QSqlDatabase::removeDatabase("XP");
    }
}

void MainWindow::makeUpdate()
{
    //    switch (currentStatus)
    //    {
    //    case 0:
    //        fillTicketViewModel(formTicketQuery(InWork,100));
    //        break;
    //    case 1:
    //        fillTicketViewModel(formTicketQuery(Ready,100));
    //        break;
    //    case 2:
    //        fillTicketViewModel(formTicketQuery(Closed,100));
    //        break;
    //    default:
    //        sb("shit happens");
    //        break;
    //    }
    model->fetchMore();
    updateTableViewTicket->start(DEFAULTPERIOD);
    sb("timer event!");
}

QString MainWindow::formTicketQuery(int ticketStatus, int limit)
{
    if (currentStatus==Closed)
        return "select ticket_id,ticket_date_in,(select branch_name from branches where id=ticket_branch),ticket_fio,ticket_phone,ticket_device,ticket_problem,ticket_price,ticket_date_out from Ticket where ticket_status="+QString::number(Closed)+" ORDER BY Ticket_ID DESC ";//LIMIT "+QString::number(limit);

    return "select ticket_id,ticket_date_in,(select branch_name from branches where id=ticket_branch),ticket_fio,ticket_phone,ticket_device,ticket_problem from Ticket where ticket_status="+QString::number(ticketStatus)+" ORDER BY Ticket_ID DESC ";//LIMIT "+QString::number(limit)
}

void MainWindow::sb(QString text)
{
    ui->statusBar->showMessage(text,10000);
}

void MainWindow::genReport(const int &type)
{
    QSqlQuery q(QSqlDatabase::database(CONNECTIONNAME, false));
    if (!SetupManager::instance()->getSqlQueryForDB(q))
        return;
    q.prepare("select ticket_fio,ticket_phone,ticket_device,ticket_problem,ticket_date_in,ticket_serial,ticket_qual from Ticket where ticket_id = ?");
    q.addBindValue(type);
    if (!q.exec())
        return;
    if (!q.next())
        return;
    QStringList list;
    QString data;
    data.append(q.value(0).toString());
    data.append(";");
    data.append(q.value(1).toString());
    data.append(";");
    data.append(q.value(2).toString());
    data.append(";");
    data.append(q.value(3).toString());
    data.append(";");
    data.append(q.value(4).toString());
    data.append(";");
    data.append(q.value(5).toString());
    data.append(";");
    data.append(q.value(6).toString());
    data.append(";");
    data.append(QString::number(type));
    list << data;


    NCReport *report = new NCReport();

    report->setReportSource( NCReportSource::File );
    report->setReportFile("./Reports/report.xml");
    report->addStringList(list, "model1");

    report->runReportToPreview();

    if (report->hasError()) {

        qDebug() << "ERROR:" << report->lastErrorMsg();

    } else {

        NCReportPreviewWindow *pv = new NCReportPreviewWindow();
        pv->setOutput( (NCReportPreviewOutput*)report->output() );
        pv->setWindowModality( Qt::ApplicationModal );
        pv->setAttribute( Qt::WA_DeleteOnClose );
        pv->show();
    }

    delete report;
}

void MainWindow::fillTicketViewModel(QString query)
{
    QSqlQuery q(QSqlDatabase::database(CONNECTIONNAME, false));
    if (!SetupManager::instance()->getSqlQueryForDB(q))
        return;
    q.prepare(query);

    if (!q.exec())
        qDebug() << "случилась какая-то неведомая херня";
    model->setQuery(q);
    model->query().exec();

    model->setHeaderData(0, Qt::Horizontal, tr("№"));            //0
    model->setHeaderData(1, Qt::Horizontal, tr("Дата"));          //1
    model->setHeaderData(2,Qt::Horizontal,tr("Филиал"));
    model->setHeaderData(3, Qt::Horizontal, tr("ФИО"));      //2
    model->setHeaderData(4, Qt::Horizontal, tr("Телефон"));   //3
    model->setHeaderData(5, Qt::Horizontal, tr("Устройство"));    //4
    model->setHeaderData(6, Qt::Horizontal, tr("Неисправность"));          //5
    ui->tableViewTicket->setModel(model);
    ui->tableViewTicket->resizeColumnToContents(3);
    if (currentStatus==Closed)
    {
        model->setHeaderData(7, Qt::Horizontal, tr("Цена"));//6
        model->setHeaderData(8, Qt::Horizontal, tr("Выдано"));
        ui->tableViewTicket->resizeColumnsToContents();
    }
    else
    {
        ui->tableViewTicket->setColumnWidth(1,70);
        ui->tableViewTicket->setColumnWidth(4,120);
        ui->tableViewTicket->setColumnWidth(5,200);
    }

}

bool MainWindow::checkDbConnection()
{
    if (QSqlDatabase::database(CONNECTIONNAME).isOpen() && QSqlDatabase::database(CONNECTIONNAME).isValid())
        return true;
    else
        return false;
}

bool MainWindow::checkDbSettings()
{
    //    if (SetupManager::instance()->getCurrentUser() == "")
    //        return false;
    if (SetupManager::instance()->getDbHostName() == "")
        return false;
    if (SetupManager::instance()->getDbName() == "")
        return false;
    if (QString::number(SetupManager::instance()->getDbPort()) == "")
        return false;
    if (SetupManager::instance()->getDbUserName() == "")
        return false;
    if (SetupManager::instance()->getDbSQLStatus() != SetupManager::FBCorrect)
        return false;
    return true;
}

bool MainWindow::connectToDb(QString dbConnectionName)
{
    QSettings settings;
    SetupManager::instance()->setDbHostName(settings.value("db/HostName").toString().trimmed());
    SetupManager::instance()->setDbName(settings.value("db/DatabaseName").toString().trimmed());
    QByteArray ba = settings.value("db/Password").toByteArray();
    SetupManager::encryptDecrypt(ba);
    SetupManager::instance()->setDbPassword(QString::fromUtf8(ba.data(), ba.count()));
    //SetupManager::instance()->setDbPort(settings.value("db/Port").toString());
    SetupManager::instance()->setDbUserName(settings.value("db/UserName").toString().trimmed());
    if (SetupManager::instance()->openSQLDatabase(dbConnectionName) != SetupManager::FBCorrect)
    {
        qDebug() << "failed to connectToDb, fbStatus =" << SetupManager::instance()->getDbSQLStatus();
        return false;
    }
    return true;
}

bool MainWindow::disconnectFromDb(QString dbConnectionName)
{
    try
    {
        QSqlDatabase::database(dbConnectionName).close();
        QSqlDatabase::removeDatabase(dbConnectionName);
        model->removeColumns(0,model->columnCount());
        return true;
    }
    catch(...)
    {
        qDebug() << "something bad happend during disconnect";
    }
    return false;
}

bool MainWindow::settingsIsNotEmpty()
{
    QSettings settings;
    return settings.allKeys().count();
}

void MainWindow::onSettingsClicked()
{   
    ConnectDialog cd(this);
    if (cd.exec())
    {
        ui->actionConnect->setEnabled(true);
        on_actionConnect_triggered();
    }
}

void MainWindow::onAddReceiptClicked()
{

    QString dbConnectionString = "MSADDRECEIPT";
    {
        if (SetupManager::instance()->openSQLDatabase(dbConnectionString) != SetupManager::FBCorrect)
        {
            QSqlDatabase::removeDatabase(dbConnectionString);
            return;
        }

        QSqlDatabase db = QSqlDatabase::database(dbConnectionString, false);
        if (!db.isOpen())
        {
            qDebug() << "Error! database not open";
            QSqlDatabase::removeDatabase(dbConnectionString);
            return;
        }

        db.transaction();
        ReceiptManager rm("MSADDRECEIPT",this);
        updateTableViewTicket->stop();
        if (rm.exec())
        {
            db.commit();
            fillTicketViewModel(formTicketQuery(InWork,LIMIT));
            ui->radioButtonWorking->setChecked(true);
            qDebug() << "got db.commit";
        }
        else
        {
            db.rollback();
            qDebug() << "rollback?";
        }
        updateTableViewTicket->start(DEFAULTPERIOD);
        db.close();
    }
    QSqlDatabase::removeDatabase(dbConnectionString);
}

void MainWindow::onJobListClicked()
{

    QString dbConnectionString = "MSJOBLIST";
    {
        if (SetupManager::instance()->openSQLDatabase(dbConnectionString) != SetupManager::FBCorrect)
        {
            QSqlDatabase::removeDatabase(dbConnectionString);
            return;
        }

        QSqlDatabase db = QSqlDatabase::database(dbConnectionString, false);
        if (!db.isOpen())
        {
            qDebug() << "Error! database not open MSJOBLIST";
            QSqlDatabase::removeDatabase(dbConnectionString);
            return;
        }

        db.transaction();
        JobListOnReceiptDialog jlord(dbConnectionString,model->record(ui->tableViewTicket->currentIndex().row()).value(0).toInt(),this);
        updateTableViewTicket->stop();
        if (jlord.exec())
        {
            db.commit();
            // fillTicketViewModel(formTicketQuery(InWork,LIMIT));
            // ui->radioButtonWorking->setChecked(true);
            qDebug() << "got db.commit";
        }
        else
        {
            db.rollback();
            qDebug() << "rollback";
        }
        updateTableViewTicket->start(DEFAULTPERIOD);
        db.close();
    }
    QSqlDatabase::removeDatabase(dbConnectionString);
    fillTicketViewModel(formTicketQuery(currentStatus,LIMIT));
}

void MainWindow::on_radioButtonReady_pressed()
{
    if (checkDbConnection())
    {
        currentStatus = Ready;
        fillTicketViewModel(formTicketQuery(currentStatus,LIMIT));
    }
}

void MainWindow::on_radioButtonWorking_pressed()
{
    if (checkDbConnection())
    {
        currentStatus = InWork;
        fillTicketViewModel(formTicketQuery(currentStatus,LIMIT));
    }
}

void MainWindow::on_radioButtonClosed_pressed()
{
    if (checkDbConnection())
    {
        currentStatus = Closed;
        fillTicketViewModel(formTicketQuery(currentStatus,LIMIT));
    }
}

void MainWindow::on_tableViewTicket_clicked(const QModelIndex &index)
{
    //    QModelIndex firstcolumn = model->index(index.row(),0);
    //    sb(model->data(firstcolumn).toString());
    currentTicket = model->record(index.row()).value(0).toInt();
    sb(QString::number(currentTicket));
}

void MainWindow::on_pushButtonSearchClear_clicked()
{
    ui->lineEdit->clear();
    ui->radioButtonWorking->setChecked(true);
    fillTicketViewModel(formTicketQuery(InWork,100));
    currentStatus = InWork;
    updateTableViewTicket->start(DEFAULTPERIOD);
}

void MainWindow::on_pushButtonSearch_clicked()
{
    updateTableViewTicket->stop();
    fillTicketViewModel(
                "select ticket_id,ticket_date_in,ticket_fio,ticket_phone,ticket_device,ticket_problem from Ticket where Ticket_ID LIKE '%"+ui->lineEdit->text()+"%' or Ticket_FIO LIKE '%"+ui->lineEdit->text()+"%' ORDER BY Ticket_ID DESC"
                );
}

void MainWindow::on_radioButtonReady_toggled(bool checked)
{
    if (checked)
        currentStatus = Ready;
}

void MainWindow::on_radioButtonWorking_toggled(bool checked)
{
    if (checked)
        currentStatus = InWork;
}

void MainWindow::on_radioButtonClosed_toggled(bool checked)
{
    if (checked)
        currentStatus = Closed;
}

void MainWindow::on_tableViewTicket_doubleClicked(const QModelIndex &index)
{
    currentTicket = model->record(index.row()).value(0).toInt();
    QString dbConnectionString = "MSTICKETVIEW";
    {
        if (SetupManager::instance()->openSQLDatabase(dbConnectionString) != SetupManager::FBCorrect)
        {
            qDebug() << "removedb on view ticket";
            QSqlDatabase::removeDatabase(dbConnectionString);
            return;
        }

        QSqlDatabase db = QSqlDatabase::database(dbConnectionString, false);
        if (!db.isOpen())
        {
            qDebug() << "Error! database not open";
            QSqlDatabase::removeDatabase(dbConnectionString);
            return;
        }

        db.transaction();
        updateTableViewTicket->stop();
        ReceiptManager rm(dbConnectionString,currentTicket,this);
        if (rm.exec())
        {
            db.commit();
            fillTicketViewModel(formTicketQuery(currentStatus,LIMIT));
        }
        else
            db.rollback();
        updateTableViewTicket->start(DEFAULTPERIOD);
        db.close();
    }
    QSqlDatabase::removeDatabase(dbConnectionString);
}

void MainWindow::on_actionConnect_triggered()
{
    if (!connectToDb(CONNECTIONNAME))
    {
        qDebug() << "failed to connect to DB on action connect";
        return;
    }
    QSqlQuery q;
    if (!SetupManager::instance()->getSqlQueryForDB(q))
    {
        qDebug() << "failed to get sql query for db on actionConnect, check db settings";
        return;
    }

    fillTicketViewModel(formTicketQuery(InWork,LIMIT));
}

void MainWindow::on_actionDisconnect_triggered()
{
    disconnectFromDb(CONNECTIONNAME);
    if (settingsIsNotEmpty())
        ui->actionConnect->setEnabled(true);
    else
        ui->actionConnect->setEnabled(false);
}

void MainWindow::on_actionPrintTicket_triggered()
{
    genReport(currentTicket);
}

void MainWindow::on_actionBranches_triggered()
{
    QString dbConnectionString = "MSBRANCHESVIEW";
    {
        if (SetupManager::instance()->openSQLDatabase(dbConnectionString) != SetupManager::FBCorrect)
        {
            qDebug() << "removedb on branches";
            QSqlDatabase::removeDatabase(dbConnectionString);
            return;
        }

        QSqlDatabase db = QSqlDatabase::database(dbConnectionString, false);
        if (!db.isOpen())
        {
            qDebug() << "Error! database not open";
            QSqlDatabase::removeDatabase(dbConnectionString);
            return;
        }

        db.transaction();
        updateTableViewTicket->stop();
        BranchWidget bw(dbConnectionString,this);
        if (bw.exec())
        {
            db.commit();
        }
        else
            db.rollback();
        updateTableViewTicket->start(DEFAULTPERIOD);
        db.close();
    }
    QSqlDatabase::removeDatabase(dbConnectionString);
    fillTicketViewModel(formTicketQuery(currentStatus,LIMIT));
}

void MainWindow::on_actionCloseTicket_triggered()
{
    QString dbConnectionString = "MSCLOSETICKET";
    {
        if (SetupManager::instance()->openSQLDatabase(dbConnectionString) != SetupManager::FBCorrect)
        {
            qDebug() << "removedb on closeticket";
            QSqlDatabase::removeDatabase(dbConnectionString);
            return;
        }

        QSqlDatabase db = QSqlDatabase::database(dbConnectionString, false);
        if (!db.isOpen())
        {
            qDebug() << "Error! database not open";
            QSqlDatabase::removeDatabase(dbConnectionString);
            return;
        }

        db.transaction();
        updateTableViewTicket->stop();
        if (currentTicket <= 0 )
            return;
        CloseTicketWidget ctw(dbConnectionString,currentTicket,this);
        if (ctw.exec())
        {
            db.commit();
        }
        else
            db.rollback();
        updateTableViewTicket->start(DEFAULTPERIOD);
        db.close();
    }
    QSqlDatabase::removeDatabase(dbConnectionString);
    fillTicketViewModel(formTicketQuery(currentStatus,LIMIT));
}

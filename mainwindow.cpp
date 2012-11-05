#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "receiptmanager.h"
#include "joblistonreceiptdialog.h"
#include "connectdialog.h"
#include "setupmanager.h"

#include "ncreport/include/ncreport.h"
#include "ncreport/include/ncreporthtmloutput.h"
#include "ncreport/include/ncreportpreviewoutput.h"
#include "ncreport/include/ncreportpreviewwindow.h"

#include <QNetworkConfigurationManager>
#include <QNetworkConfiguration>
#include <QSqlQueryModel>
#include <QFileDialog>
#include <QPrinter>
#include <QPainter>
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

    if (!checkDbSettings())
    {
        ui->actionConnect->setEnabled(false);
    }

    on_actionConnect_triggered();

    connect(updateTableViewTicket,SIGNAL(timeout()),this, SLOT(makeUpdate()));

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
    /*QString dbConnectionString = "XP";
    QSqlDatabase db = QSqlDatabase::database(dbConnectionString, false);
    if (db.isOpen())
        db.close();
    QSqlDatabase::removeDatabase(dbConnectionString);*/
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
    switch (currentStatus)
    {
    case 0:
        fillTicketViewModel(formTicketQuery(InWork,100));
        break;
    case 1:
        fillTicketViewModel(formTicketQuery(Ready,100));
        break;
    case 2:
        fillTicketViewModel(formTicketQuery(Closed,100));
        break;
    default:
        sb("shit happens");
        break;
    }
    updateTableViewTicket->start(DEFAULTPERIOD);
    sb("timer event!");
}

QString MainWindow::formTicketQuery(int ticketStatus, int limit)
{
    return "select ticket_id,ticket_date_in,ticket_fio,ticket_phone,ticket_device,ticket_problem from Ticket where ticket_status="+QString::number(ticketStatus)+" ORDER BY `Ticket_ID` DESC LIMIT "+QString::number(limit);
}

void MainWindow::sb(QString text)
{
    ui->statusBar->showMessage(text,10000);
}

void MainWindow::genReport(const int &type)
{
    NCReport* report = new NCReport();
//    NCReportPreviewOutput* output = new NCReportPreviewOutput();
//    report->setReportFile("test.pdf");
//    report->addParameter("id",12);
//    report->setParseMode( NCReport::fromFile );
//    report->setFileEncoding( "ISO8859-2");
//    report->setShowPrintDialog( true );
//    report->setPreviewAsMain( true );	// preview is the main form on preview mode
//    report->setDeleteReportAfterPreview( true );	// delete report object after close preview
//    report->setPreviewIsMaximized( false );
//report->setOutput( );
//    report->runReportToPreview();
report->setReportFile("./itemmodel_demo.xml");
    report->addItemModel(model,"model1");
//     NCReportPreviewWindow* pv = new NCReportPreviewWindow;
//         pv->setReport(report);
//       //  pv->setOutput((NCReportPreviewOutput*)output);
//         pv->show();

//    report->runReportToQtPreview();
    //NCReportPreviewWindow window;
    //window.setReport( report );

    //window.show();
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
    model->setHeaderData(2, Qt::Horizontal, tr("ФИО"));      //2
    model->setHeaderData(3, Qt::Horizontal, tr("Телефон"));   //3
    model->setHeaderData(4, Qt::Horizontal, tr("Устройство"));    //4
    model->setHeaderData(5, Qt::Horizontal, tr("Неисправность"));          //5
    model->setHeaderData(6, Qt::Horizontal, tr("Неисправность"));//6
    ui->tableViewTicket->setModel(model);
    ui->tableViewTicket->resizeColumnToContents(2);
    ui->tableViewTicket->setColumnWidth(1,70);
    ui->tableViewTicket->setColumnWidth(3,120);
    ui->tableViewTicket->setColumnWidth(4,200);
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
    SetupManager::instance()->setDbPort(settings.value("db/Port").toString());
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
        qDebug() << "succesfully removed database " << dbConnectionName;
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
        ReceiptManager rm("MSADDRECEIPT",0,this);
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
}

void MainWindow::on_radioButtonReady_pressed()
{
    if (checkDbConnection())
    {
        fillTicketViewModel(formTicketQuery(Ready,LIMIT));
        currentStatus = Ready;
    }
}

void MainWindow::on_radioButtonWorking_pressed()
{
    if (checkDbConnection())
    {
        fillTicketViewModel(formTicketQuery(InWork,LIMIT));
        currentStatus = InWork;
    }
}

void MainWindow::on_radioButtonClosed_pressed()
{
    if (checkDbConnection())
    {
        fillTicketViewModel(formTicketQuery(Closed,LIMIT));
        currentStatus = Closed;
    }
}

void MainWindow::on_tableViewTicket_clicked(const QModelIndex &index)
{
    QModelIndex firstcolumn = model->index(index.row(),0);
    sb(model->data(firstcolumn).toString());
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
        ReceiptManager rm(dbConnectionString,model->record(index.row()).value(0).toInt(),this);
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
    genReport(0);
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "receiptmanager.h"
#include "joblistonreceiptdialog.h"
#include "connectdialog.h"
#include "setupmanager.h"
#include "branchwidget.h"

#include "ncreport/include/ncreport.h"
#include "ncreport/include/ncreportoutput.h"
#include "ncreport/include/ncreportpreviewoutput.h"
#include "ncreport/include/ncreportpreviewwindow.h"

#include <QNetworkConfigurationManager>
#include <QNetworkConfiguration>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>

const QString CONNECTIONNAME = "XP";
const int DEFAULTPERIOD = 5000;
const int LIMIT = 1000;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    jobModel = new QStandardItemModel(this);
    ui->treeViewJobsOnTicket->setModel(jobModel);
    model = new QSqlQueryModel(ui->tableViewTicket);
    ui->groupBoxFastTicketInfo->setVisible(false);
    QNetworkConfigurationManager* qnam= new QNetworkConfigurationManager();

    defaultConfName =  qnam->defaultConfiguration().name();
    connect(qnam,SIGNAL(configurationChanged(QNetworkConfiguration)),this,SLOT(networkFuckedUpTwo(QNetworkConfiguration)));

    model = new QSqlQueryModel(ui->tableViewTicket);
    proxy = new QSortFilterProxyModel(this);
    proxy->setSourceModel(model);
    ui->tableViewTicket->setModel(proxy);
    ui->tableViewTicket->setContextMenuPolicy(Qt::CustomContextMenu);
    updateTableViewTicket = new QTimer(this);
    currentStatus = InWork;
    connect(ui->tableViewTicket, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onCustomContextMenuRequested(QPoint)));
    connect(ui->actionOnAddReceiptClicked, SIGNAL(triggered()), this, SLOT(onAddReceiptClicked()));
    connect(ui->actionOnJobListClicked,SIGNAL(triggered()),this,SLOT(onJobListClicked()));
    connect(ui->actionExitMenuClicked,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionSettingsMenuClicked,SIGNAL(triggered()),this,SLOT(onSettingsClicked()));
    connect(ui->actionBranchTriggered,SIGNAL(triggered()),this,SLOT(onActionBranchesTriggered()));
    connect(ui->actionCloseTicket,SIGNAL(triggered()),this,SLOT(on_actionCloseTicket_triggered()));
    connect(ui->tableViewTicket->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(onTableViewTicketSelectionChanged(QModelIndex,QModelIndex)));
    ui->actionOnJobListClicked->setEnabled(false);
    ui->actionCloseTicket->setEnabled(false);
    ui->actionConnect->setEnabled(checkDbSettings());

    on_actionConnect_triggered();

    connect(updateTableViewTicket,SIGNAL(timeout()),this,SLOT(makeUpdate()));
   // updateTableViewTicket->start(DEFAULTPERIOD);
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
        switch (currentStatus)
        {
        case 1:
            fillTicketViewModel(formTicketQuery(InWork,LIMIT));
            break;
        case 0:
            fillTicketViewModel(formTicketQuery(Ready,LIMIT));
            break;
        case 2:
            fillTicketViewModel(formTicketQuery(Closed,LIMIT));
            break;
        default:
            sb("shit happens");
            break;
        }
  //  updateTableViewTicket->start(DEFAULTPERIOD);
}

QString MainWindow::formTicketQuery(int ticketStatus, int limit)
{
    if (currentStatus==Closed)
        return "select ticket_id,ticket_date_in, branch_name, ticket_fio,ticket_phone,ticket_device, "
                "ticket_problem,ticket_price,ticket_date_out from Ticket join branch "
                "on(ticket.ticket_branch = branch.id) where ticket_status="+QString::number(Closed)+" ORDER BY Ticket_ID DESC LIMIT "+QString::number(limit);
    return "select ticket_id, ticket_date_in, branch_name, ticket_fio,ticket_phone,ticket_device, "
            "ticket_problem from Ticket join branch on(ticket.ticket_branch = branch.id) "
            "where ticket_status="+QString::number(ticketStatus)+" ORDER BY Ticket_ID DESC LIMIT "+QString::number(limit);
}

QVariant MainWindow::getCurrentTicketId()
{
    return model->record(proxy->mapToSource(ui->tableViewTicket->currentIndex()).row()).value(0);
}

void MainWindow::sb(QString text)
{
    ui->statusBar->showMessage(text,10000);
}

void MainWindow::genReport(const int &type)
{
    Q_UNUSED(type);
  /*  QSqlQuery q(QSqlDatabase::database(CONNECTIONNAME, false));
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
    report->setReportFile("./report.xml");
    //report->addItemModel(model,"model1");
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

    delete report;*/
}

void MainWindow::fillTicketViewModel(QString query)
{
    model->clear();

    QSqlQuery q(QSqlDatabase::database(CONNECTIONNAME, false));
    if (!SetupManager::instance()->getSqlQueryForDB(q))
        return;

    q.prepare(query);

    if (!q.exec())
    {
        qDebug() << q.lastError();
        return;
    }

    model->setQuery(q);
    model->setHeaderData(0, Qt::Horizontal, tr("№"));            //0
    model->setHeaderData(1, Qt::Horizontal, tr("Дата"));          //1
    model->setHeaderData(2, Qt::Horizontal,tr("Филиал"));
    model->setHeaderData(3, Qt::Horizontal, tr("ФИО"));      //2
    model->setHeaderData(4, Qt::Horizontal, tr("Телефон"));   //3
    model->setHeaderData(5, Qt::Horizontal, tr("Устройство"));    //4
    model->setHeaderData(6, Qt::Horizontal, tr("Неисправность"));
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
    ui->groupBoxFastTicketInfo->setVisible(false);
}

bool MainWindow::checkDbConnection()
{
    return (QSqlDatabase::database(CONNECTIONNAME).isOpen() && QSqlDatabase::database(CONNECTIONNAME).isValid());
}

bool MainWindow::checkDbSettings()
{
    return (!SetupManager::instance()->getDbHostName().isEmpty() &&
            !SetupManager::instance()->getDbName().isEmpty() &&
            !QString::number(SetupManager::instance()->getDbPort()).isEmpty() &&
            !SetupManager::instance()->getDbUserName().isEmpty() &&
            SetupManager::instance()->getDbSQLStatus() == SetupManager::FBCorrect);
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
    return (SetupManager::instance()->openSQLDatabase(dbConnectionName) == SetupManager::FBCorrect);
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
        }
        else        
            db.rollback();

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
            QSqlDatabase::removeDatabase(dbConnectionString);
            return;
        }

        db.transaction();

        JobListOnReceiptDialog jlord(dbConnectionString,getCurrentTicketId().toInt(),this);
        updateTableViewTicket->stop();
        if (jlord.exec())        
            db.commit();        
        else        
            db.rollback();        
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
    currentTicket = model->data(index).toInt();
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
                "select ticket_id,ticket_date_in, branch_name, ticket_fio,ticket_phone,ticket_device, "
                "ticket_problem from Ticket join branch on(ticket.ticket_branch = branch.id) "
                "where cast(ticket.ticket_id AS Text) LIKE ('%"+ui->lineEdit->text().trimmed()+"%') "
                "or Ticket_FIO LIKE ('%"+ui->lineEdit->text().trimmed()+"%') ORDER BY Ticket_ID DESC"
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

void MainWindow::onTableViewTicketSelectionChanged(QModelIndex current, QModelIndex previous)
{
    Q_UNUSED(previous);
    ui->actionOnJobListClicked->setEnabled(current.isValid());
    ui->actionCloseTicket->setEnabled(current.isValid());
    jobModel->clear();
    jobModel->setHorizontalHeaderLabels(QStringList() << trUtf8("Мастер") << trUtf8("Наименование")
                                     << trUtf8("Количество") << trUtf8("Цена") << trUtf8("Дата"));
    int totalPrice = 0;
    QSqlQuery q;
    if (SetupManager::instance()->getSqlQueryForDB(q))
    {
        q.prepare("select employee_FIO,job_name,job_quantity,job_price,Job_date,jot_id from JobOnTicket join Employee ON (JobOnTicket.Employee_ID=Employee.Employee_ID) where Ticket_ID=?");
        q.addBindValue(model->record(proxy->mapToSource(current).row()).value(0));
        q.exec();
        while (q.next())
        {
            QStandardItem* fio = new QStandardItem(q.value(0).toString());
            fio->setData(q.value(5));
            QStandardItem* jobName = new QStandardItem(q.value(1).toString());
            QStandardItem* jobQuant = new QStandardItem(q.value(2).toString());
            QStandardItem* jobPrice = new QStandardItem(q.value(3).toString());
            QStandardItem* jobDate = new QStandardItem(q.value(4).toString());
            totalPrice += q.value(3).toInt();
            jobModel->appendRow(QList<QStandardItem*>() << fio << jobName << jobQuant << jobPrice << jobDate);
        }
        ui->groupBoxFastTicketInfo->setVisible(jobModel->rowCount() != 0);
        ui->groupBoxFastTicketInfo->setMaximumHeight(height()*0.2);
        ui->lineEditTicketPrice->setText(QString::number(totalPrice));
        ui->groupBoxFastTicketInfo->setTitle(trUtf8("Квитанция № %0").arg(getCurrentTicketId().toString()));
        for (int i = 0 ; i < jobModel->columnCount(); ++i)
            ui->treeViewJobsOnTicket->resizeColumnToContents(i);

    }
}

void MainWindow::onCustomContextMenuRequested(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);
    connect(menu, SIGNAL(aboutToHide()), menu, SLOT(deleteLater()));
    QModelIndex ind = ui->tableViewTicket->indexAt(pos);
    menu->addAction(trUtf8("Добавить квитанцию"), this, SLOT(onAddReceiptClicked()));
    if (ind.isValid())
    {
        menu->addAction(trUtf8("Список работ"), this, SLOT(onJobListClicked()));
        if (currentStatus != Closed)
            menu->addAction(trUtf8("Закрыть квитанцию"), this, SLOT(on_actionCloseTicket_triggered()));
        if (currentStatus != InWork)
            menu->addAction(trUtf8("Вернуть в работу"), this, SLOT(onMoveBackToWork()));
        if (currentStatus != Ready)
            menu->addAction(trUtf8("Поместить в готовые"), this, SLOT(onMoveBackToReady()));
    }
    menu->exec(QCursor::pos());
}

void MainWindow::onMoveBackToWork()
{
    QSqlQuery q;
    if (!SetupManager::instance()->getSqlQueryForDB(q))
        return;
    q.prepare("update ticket set ticket_status = 1 where ticket_id = ?");
    q.addBindValue(getCurrentTicketId());
    q.exec();
    fillTicketViewModel(formTicketQuery(currentStatus,LIMIT));
}

void MainWindow::onMoveBackToReady()
{
    QSqlQuery q;
    if (!SetupManager::instance()->getSqlQueryForDB(q))
        return;
    q.prepare("update ticket set ticket_status = 0 where ticket_id = ?");
    q.addBindValue(getCurrentTicketId());
    q.exec();
    fillTicketViewModel(formTicketQuery(currentStatus,LIMIT));
}

void MainWindow::on_actionConnect_triggered()
{
    if (!connectToDb(CONNECTIONNAME))
    {
        qDebug() << "failed to connect to DB on action connect";
        return;
    }    
    fillTicketViewModel(formTicketQuery(InWork,LIMIT));
}

void MainWindow::on_actionDisconnect_triggered()
{
    disconnectFromDb(CONNECTIONNAME);
    ui->actionConnect->setEnabled(settingsIsNotEmpty());
}

void MainWindow::on_actionPrintTicket_triggered()
{
    genReport(currentTicket);
}

void MainWindow::onActionBranchesTriggered()
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
    QSqlQuery q;
    if (!SetupManager::instance()->getSqlQueryForDB(q))
        return;
    q.prepare("update ticket set ticket_status = 2 where ticket_id = ?");
    q.addBindValue(getCurrentTicketId());
    q.exec();
    fillTicketViewModel(formTicketQuery(currentStatus,LIMIT));
}

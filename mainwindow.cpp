#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "receiptmanager.h"
#include "joblistonreceiptdialog.h"
#include "connectdialog.h"
#include "setupmanager.h"
#include "branchwidget.h"
#include "changeuserdialog.h"
#include "usermanagementdialog.h"
#include "userstatisticwidget.h"
#include "productcategorymanager.h"
#include "globals.h"
#include "guaranteeonticketreasonwidget.h"
#include "reportshandler.h"

#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QWidgetAction>
#include <QStandardItem>

const QString CONNECTIONNAME = "XP";
const int DEFAULTPERIOD = 5000;
const int STATUSBARTIMEOUT = 10000;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    jobModel = new QStandardItemModel(this);
    ui->treeViewJobsOnTicket->setModel(jobModel);       
    ui->groupBoxFastTicketInfo->setVisible(false);

    ticketModel = new QStandardItemModel(this);
    ticketProxy = new QSortFilterProxyModel(this);
    ticketProxy->setSourceModel(ticketModel);
    ticketProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->tableViewTicket->setModel(ticketProxy);
    ui->tableViewTicket->setContextMenuPolicy(Qt::CustomContextMenu);
    updateTableViewTicket = new QTimer(this);
    currentStatus = InWork;

    connect(ui->tableViewTicket, SIGNAL(customContextMenuRequested(QPoint)), SLOT(onCustomContextMenuRequested(QPoint)));
    connect(ui->actionOnAddReceiptClicked, SIGNAL(triggered()), SLOT(onAddTicketClicked()));
    connect(ui->actionOnJobListClicked,SIGNAL(triggered()), SLOT(onJobListClicked()));
    connect(ui->actionExitMenuClicked,SIGNAL(triggered()), SLOT(close()));
    connect(ui->actionSettingsMenuClicked,SIGNAL(triggered()), SLOT(onSettingsClicked()));
    connect(ui->actionBranchTriggered,SIGNAL(triggered()), SLOT(onActionBranchesTriggered()));
    connect(ui->actionCloseTicket,SIGNAL(triggered()), SLOT(onCloseTicketClicked()));
    connect(ui->actionChangeUser, SIGNAL(triggered()), SLOT(onActionChangeUserClicked()));
    connect(ui->actionUserControl, SIGNAL(triggered()), SLOT(onActionUserManagementClicked()));
    connect(ui->actionAddProductCategory, SIGNAL(triggered()), SLOT(onActionCategoryProductsClicked()));
    connect(ui->actionPrintTicket, SIGNAL(triggered()), SLOT(onGenerateTicketReport()));
    connect(ui->tableViewTicket->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(onTableViewTicketSelectionChanged(QModelIndex,QModelIndex)));
    connect(ui->tableViewTicket, SIGNAL(clicked(QModelIndex)), SLOT(onIsClientNotifiedClicked(QModelIndex)));
    ui->actionOnJobListClicked->setEnabled(false);
    ui->actionCloseTicket->setEnabled(false);
    ui->actionConnect->setEnabled(checkDbSettings());
    changePermissions();
    on_actionConnect_triggered();

    connect(ui->tabWidget, SIGNAL(currentChanged(int)), SLOT(onTabChanged(int)));

    cud = new ChangeUserDialog(this);
    QWidgetAction* wa = new QWidgetAction(this);
    wa->setDefaultWidget(cud);
    connect(cud, SIGNAL(accepted()), SLOT(onChangeUserInPopupMenu()));
    connect(cud, SIGNAL(rejected()), SLOT(onRejectUserInPopupMenu()));
    connect(cud, SIGNAL(changePermissions()), SLOT(changePermissions()));
    ui->menuCurrentUser->addAction(wa);

    //do NOT delete it... yet.
    //connect(updateTableViewTicket,SIGNAL(timeout()),this,SLOT(refreshTicketModel()));
    ticketComments = new QStandardItemModel(this);
    ui->treeViewTicketComments->setModel(ticketComments);
    ui->treeViewTicketComments->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeViewTicketComments->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->treeViewTicketComments, SIGNAL(customContextMenuRequested(QPoint)), SLOT(onCommentsCustomContextMenuRequested(QPoint)));

    connect(ui->pushButtonNotAGuarantee, SIGNAL(clicked()), SLOT(onNotAGuaranteeClicked()));
    connect(ui->pushButtonAcceptToWorkGuarantee, SIGNAL(clicked()), SLOT(onAcceptAGuaranteeClicked()));

    cud->accept();

    productModel = new QSqlQueryModel(this);
    proCatModel = new QStandardItemModel(this);
    proxyProduct = new QSortFilterProxyModel(this);
    proxyProduct->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->treeViewCategory->setModel(proCatModel);
    proxyProduct->setSourceModel(productModel);
    ui->tableViewProducts->setModel(proxyProduct);
    connect(ui->treeViewCategory->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(onCurrentCategoryChanged(QModelIndex,QModelIndex)));
    connect(this, SIGNAL(refreshProductModelByCategory(int)), SLOT(onRefreshProductByType(int)));
    connect(ui->lineEditFilter, SIGNAL(textChanged(QString)), proxyProduct, SLOT(setFilterFixedString(QString)));
    onRefreshCategoryModel();

    connect(ui->pushButtonAddComment, SIGNAL(clicked()), SLOT(onAddCommentToTicketClicked()));

    connect(ui->queryLimitComboBoxWidget, SIGNAL(limitChanged(int)), SLOT(onQueryLimitComboBoxIndexChanged(int)));
#ifdef OS_MAC
    ui->menuLoginStatus->setVisible(false);
    ui->menuLoginStatus->setHidden(true);
#endif
   // updateTableViewTicket->start(DEFAULTPERIOD);
}

MainWindow::~MainWindow()
{        
    QStringList connames = QSqlDatabase::connectionNames();
    for (int i = 0 ; i < connames.count(); ++i)
    {
        QSqlDatabase::database(connames.value(i)).close();
        QSqlDatabase::removeDatabase(connames.value(i));
    }
    delete ui;
}

QVariant MainWindow::getCurrentTDCRId()
{    
    if (ui->tableViewTicket->currentIndex().isValid())
        return ticketModel->item(ticketProxy->mapToSource(ui->tableViewTicket->currentIndex()).row(),TicketNumber)->data(TDC_Relation);
    else
        return QVariant();
}

QVariant MainWindow::getCurrentTDCRId(const QModelIndex &index)
{
    if (index.isValid())
        return ticketModel->item(ticketProxy->mapToSource(index).row(),TicketNumber)->data(TDC_Relation);
    else
        return QVariant();
}

void MainWindow::changeTicketStatus(const int &status)
{
    QSqlQuery q;
    if (!SetupManager::instance()->getSqlQueryForDB(q))
        return;
    QModelIndexList qmil = ui->tableViewTicket->selectionModel()->selectedIndexes();
    for (int i = 0 ; i < qmil.count(); ++i)
    {
        if (qmil.at(i).column() != TicketNumber)
            continue;
        q.prepare("update device set status = ? where device.id = (select device_id from ticket where id = ?)");
        q.addBindValue(status);
        q.addBindValue(getCurrentTDCRId(qmil.at(i)));
        if (!q.exec())
            qDebug() << q.lastError() << q.lastQuery();
    }
    refreshTicketModel(generateTicketQuery());
}

int MainWindow::getCurrentTicketId()
{
    if (ui->tableViewTicket->currentIndex().isValid())
        return ticketModel->item(ticketProxy->mapToSource(ui->tableViewTicket->currentIndex()).row(),TicketNumber)->text().toInt();
    else
        return -1;
}

int MainWindow::getCurrentTicketId(const QModelIndex &index)
{
    if (index.isValid())
        return ticketModel->item(ticketProxy->mapToSource(index).row(),TicketNumber)->text().toInt();
    else
        return -1;
}

QString MainWindow::getCurrentTicketDeviceName(const QModelIndex &index)
{
    if (index.isValid())
        return ticketModel->item(ticketProxy->mapToSource(index).row(),TicketDeviceName)->text();
    else
        return "";
}

QString MainWindow::getCurrentTicketDeviceSerial(const QModelIndex &index)
{
    if (index.isValid())
        return ticketModel->item(ticketProxy->mapToSource(index).row(),TicketDeviceSerial)->text();
    else
        return "";
}

QVariant MainWindow::getCurrentTicketGuaranteeId(const QModelIndex &index)
{
    if (index.isValid())
        return ticketModel->item(ticketProxy->mapToSource(index).row(),TicketNumber)->data(GuaranteeId);
    else
        return QVariant();
}

void MainWindow::sb(QString text)
{
    ui->statusBar->showMessage(text, STATUSBARTIMEOUT);
}

void MainWindow::changePermissions()
{
    if (SetupManager::instance()->getCurrentUser().isEmpty())
    {
        ui->menuLoginStatus->setIcon(QIcon(":/icons/icons/Remove-Male-User24.png"));
        ui->menuCurrentUser->setTitle(trUtf8("Вход не выполнен"));
    }
    else
    {
        ui->menuLoginStatus->setIcon(QIcon(":/icons/icons/Accept-Male-User24.png"));
        cud->onSuccesfullLogin();
    }
    bool permissions = SetupManager::instance()->getPermissions();
    ui->tabTickets->setEnabled(permissions);
    ui->tabShowcase->setEnabled(permissions);
    ui->menuTicket->setEnabled(permissions);
}

void MainWindow::onTabChanged(int tab)
{
    switch(tab)
    {
    case 0:
    {
        ui->menuTicket->setEnabled(true);
        foreach (QAction* act,ui->menuTicket->actions())
            act->setEnabled(true);
        ui->menuShowcase->setEnabled(false);
        foreach (QAction* act,ui->menuShowcase->actions())
            act->setEnabled(false);
        break;
    };
    case 1:
    {
        ui->menuTicket->setEnabled(false);
        foreach (QAction* act,ui->menuTicket->actions())
            act->setEnabled(false);
        ui->menuShowcase->setEnabled(true);
        foreach (QAction* act,ui->menuShowcase->actions())
            act->setEnabled((true));
        ui->groupBoxSale->setVisible(false);
        break;
    };
    }
}

void MainWindow::onRefreshCategoryModel()
{
    proCatModel->clear();
    proCatModel->setHorizontalHeaderLabels(QStringList() << trUtf8("Название"));
    QSqlQuery q;
    if (!SetupManager::instance()->getSqlQueryForDB(q))
        return;
    q.exec("select type_name, type_id from product_type");
    QStandardItem* all = new QStandardItem(trUtf8("Все"));
    all->setData(-1);
    proCatModel->appendRow(all);
    while(q.next())
    {
        QStandardItem* item = new QStandardItem(q.value(0).toString());
        item->setData(q.value(1));
        proCatModel->appendRow(item);
    }
}

void MainWindow::onRefreshProductByType(int type)
{
    productModel->clear();
    /*productModel->setHorizontalHeaderLabels(QStringList() << trUtf8("Название") <<
                                            trUtf8("Описание") << trUtf8("Цена") <<
                                            trUtf8("Количество") << trUtf8("Гарантия"));*/

    QSqlQuery q;
    if (!SetupManager::instance()->getSqlQueryForDB(q))
        return;

    if (type == -1)
        q.exec("select product_name, serial_number,  product_price, product_quant, product_guar from product left join serialforproduct on (serialforproduct.product_id = product.product_id)");
    else
    {
        q.prepare("select product_name, serial_number,  product_price, product_quant, product_guar from product left join serialforproduct on (serialforproduct.product_id = product.product_id) where type_id = ?");
        q.addBindValue(type);
        q.exec();
    }

    productModel->setQuery(q);
    productModel->setHeaderData(ProductName, Qt::Horizontal, trUtf8("Название"));
    productModel->setHeaderData(ProductSerialNumber, Qt::Horizontal, trUtf8("Серийный №"));
    productModel->setHeaderData(ProductPrice, Qt::Horizontal, trUtf8("Цена"));
    productModel->setHeaderData(ProductQuantity, Qt::Horizontal, trUtf8("Количество"));
    productModel->setHeaderData(ProductGuarantee, Qt::Horizontal, trUtf8("Гарантия"));
    ui->tableViewProducts->resizeColumnsToContents();
    sb(QString(trUtf8("Количество товаров %0").arg(productModel->rowCount())));
}

void MainWindow::onCurrentCategoryChanged(QModelIndex current, QModelIndex)
{
    if (current.isValid())
        emit refreshProductModelByCategory(proCatModel->itemFromIndex(current)->data().toInt());
}

void MainWindow::onActionCategoryProductsClicked()
{
    QString dbConnectionString = "PRODUCTCATEGORYMANAGER";
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

        ProductCategoryManager pcm(dbConnectionString,this);
        if (pcm.exec())
            db.commit();
        else
            db.rollback();

        db.close();
    }
    QSqlDatabase::removeDatabase(dbConnectionString);
    onRefreshCategoryModel();
}

QString MainWindow::generateTicketQuery()
{
    return QString("select ticket.id, ticket.ticket_id, device.date_accepted, branch.branch_name, "
                   "client.name, client.phone, device.name, device.serial, device.problem, "
                   "client_notified %0 ,ticket_guarantee.date_accepted, ticket_guarantee.id, "
                   "ticket_guarantee.date_closed "
                   "from ticket "
                   "join client on(ticket.client_id = client.id) "
                   "join device on(ticket.device_id = device.id) "
                   "join branch on(device.branch_id = branch.id) "
                   "left join ticket_guarantee on (ticket_guarantee.tdc_r_id = ticket.id) "
                   "where device.status = %1 "
                   "ORDER BY ticket.ticket_id DESC %2")
            .arg(currentStatus == Closed ? ", device.price, device.date_givenout" : "")
            .arg(QString::number(currentStatus))
            .arg(ui->queryLimitComboBoxWidget->getLimit() == 0 ? "" : QString("LIMIT ").append(QString::number(ui->queryLimitComboBoxWidget->getLimit())));
}

bool MainWindow::changeUser(const QString &login, const QString &password)
{
    QString passwordHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha1).toHex();
    QSqlQuery q;
    if (!SetupManager::instance()->getSqlQueryForDB(q))
        return false;    
    q.prepare("select password, employee_fio, employee_id, permissions from employee where login = ? and fired = FALSE");
    q.addBindValue(login);
    q.exec();
    if (!q.next())
    {
        QMessageBox::information(this, trUtf8("Ошибка!"), trUtf8("Ошибка входа!"));
        qDebug() << q.lastQuery() << login;
        return false;
    }

    int permissions = q.value(3).toInt();
    QString fio = q.value(1).toString();
    QString passwordB = q.value(0).toString().remove(0,2); //remove the \x escape character(thanks for postgress for adding it >_>)
    if (passwordHash != passwordB)
    {
        QMessageBox::information(this, trUtf8("Ошибка!"), trUtf8("Пароль не православный!"));
        return false;
    }

    currentEmployeeId = q.value(2).toInt();
    SetupManager::instance()->setCurrentUser(login);
    SetupManager::instance()->setPermissons(permissions);
    changePermissions();
    currentEmployeeName = fio;
    ui->menuCurrentUser->setTitle(fio);
    return true;
}

bool MainWindow::executeDialog(QDialog *dlg)
{
    bool ok = false;

    QString dbConnectionString = dlg->property("db").toString();

    {
        if (SetupManager::instance()->openSQLDatabase(dbConnectionString) != SetupManager::FBCorrect)
        {
            QSqlDatabase::removeDatabase(dbConnectionString);
            return false;
        }

        QSqlDatabase db = QSqlDatabase::database(dbConnectionString, false);
        if (!db.isOpen())
        {
            QSqlDatabase::removeDatabase(dbConnectionString);
            return false;
        }

        db.transaction();

        if (dlg->exec())
        {
            db.commit();
            ok = true;
        }
        else
            db.rollback();

        db.close();
    }
    QSqlDatabase::removeDatabase(dbConnectionString);
    return ok;
}

QString MainWindow::genUUID()
{
    return QUuid::createUuid().toString();
}

void MainWindow::refreshTicketModel(const QString &query)
{    
    ticketModel->clear();

    QSqlQuery q;

    if (!SetupManager::instance()->getSqlQueryForDB(q))
        return;

    q.prepare(query);

    if (!q.exec())
    {
        qDebug() << q.lastError() << q.lastQuery();
        return;
    }

    while(q.next())
    {
        QStandardItem* ticket_id = new QStandardItem(q.value(1).toString());
        ticket_id->setData(q.value(0),TDC_Relation); //ticket id
        ticket_id->setCheckable(true);
        ticket_id->setCheckState(q.value(9).toBool() ? Qt::Checked : Qt::Unchecked);
        ticket_id->setToolTip(trUtf8("Клиенту %0").arg(q.value(9).toBool() ? trUtf8("отзвонились") : trUtf8("не отзванивались")));
        QStandardItem* date_accepted = new QStandardItem(q.value(2).toDate().toString("dd-MM-yyyy"));
        date_accepted->setToolTip(date_accepted->text());
        QStandardItem* branch = new QStandardItem(q.value(3).toString());
        branch->setToolTip(q.value(3).toString());
        QStandardItem* client_name = new QStandardItem(q.value(4).toString());
        client_name->setToolTip(q.value(4).toString());
        QStandardItem* client_phone = new QStandardItem(q.value(5).toString());
        client_phone->setToolTip(q.value(5).toString());
        QStandardItem* device_name = new QStandardItem(q.value(6).toString());
        device_name->setToolTip(q.value(6).toString());
        QStandardItem* device_serial = new QStandardItem(q.value(7).toString());
        device_serial->setToolTip(q.value(7).toString());
        QStandardItem* device_problem = new QStandardItem(q.value(8).toString());
        device_problem->setToolTip(QString("<table><tr><td>").append(q.value(8).toString()).append("</td></td></table>"));
        if (currentStatus==Closed)
        {
            QStandardItem* ticket_price = new QStandardItem(q.value(9).toString());
            ticket_price->setToolTip(q.value(9).toString());            
            QStandardItem* ticket_givenout = new QStandardItem(q.value(10).toDate().toString("dd-MM-yyyy"));
            ticket_givenout->setToolTip(ticket_givenout->text());
            if (!q.value(12).toString().isEmpty() && q.value(14).toString().isEmpty())
            {
                ticket_id->setIcon(QIcon(":/icons/icons/emblem-important_9062.png"));
                ticket_id->setData(q.value(13),GuaranteeId);
            }
            ticketModel->appendRow(QList<QStandardItem*>() << ticket_id << date_accepted << branch << client_name << client_phone << device_name << device_serial << device_problem << ticket_price << ticket_givenout);
        }
        else
        {
            if (!q.value(10).toString().isEmpty() && q.value(12).toString().isEmpty())
            {
                ticket_id->setIcon(QIcon(":/icons/icons/emblem-important_9062.png"));
                ticket_id->setData(q.value(11),GuaranteeId);
            }
            ticketModel->appendRow(QList<QStandardItem*>() << ticket_id << date_accepted << branch << client_name << client_phone << device_name << device_serial << device_problem);
        }
    }

    ticketModel->setHeaderData(TicketNumber, Qt::Horizontal, tr("№"));
    ticketModel->setHeaderData(TicketDate, Qt::Horizontal, tr("Дата"));
    ticketModel->setHeaderData(TicketBranch, Qt::Horizontal, tr("Филиал"));
    ticketModel->setHeaderData(TicketClientName, Qt::Horizontal, tr("ФИО"));
    ticketModel->setHeaderData(TicketClientPhone, Qt::Horizontal, tr("Телефон"));
    ticketModel->setHeaderData(TicketDeviceName, Qt::Horizontal, tr("Устройство"));
    ticketModel->setHeaderData(TicketDeviceSerial, Qt::Horizontal, tr("Серийный №"));
    ticketModel->setHeaderData(TicketProblem, Qt::Horizontal, tr("Неисправность"));

    if (currentStatus==Closed)
    {
        ticketModel->setHeaderData(TicketPrice, Qt::Horizontal, tr("Цена"));
        ticketModel->setHeaderData(TicketGivenOut, Qt::Horizontal, tr("Выдано"));
    }
    else
    {
        ui->tableViewTicket->setColumnWidth(TicketDate,70);
        ui->tableViewTicket->setColumnWidth(TicketClientPhone,120);
        ui->tableViewTicket->setColumnWidth(TicketDeviceName,200);
        ui->tableViewTicket->resizeColumnToContents(TicketPrice);
    }

    ui->tableViewTicket->resizeColumnToContents(TicketNumber);
    ui->tableViewTicket->resizeColumnToContents(TicketDate);
    ui->tableViewTicket->resizeColumnToContents(TicketClientName);
    ui->tableViewTicket->resizeColumnToContents(TicketClientPhone);
    ui->tableViewTicket->resizeColumnToContents(TicketDeviceName);

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
        ticketModel->removeColumns(0,ticketModel->columnCount());
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

void MainWindow::onAddTicketClicked()
{
    updateTableViewTicket->stop();

    QString dbConnectionString = genUUID();

    ReceiptManager* rc = new ReceiptManager(dbConnectionString,-1,this);
    rc->setProperty("db",dbConnectionString);
    if (executeDialog(rc))
    {
        refreshTicketModel(generateTicketQuery());
        ui->radioButtonWorking->setChecked(true);
    }


    /*
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
        ReceiptManager rm(dbConnectionString,-1,this);
        if (rm.exec())
        {
            db.commit();
            refreshTicketModel(generateTicketQuery());
            ui->radioButtonWorking->setChecked(true);
        }
        else        
            db.rollback();       
        db.close();
    }*/
    updateTableViewTicket->start(DEFAULTPERIOD);
    //QSqlDatabase::removeDatabase(dbConnectionString);
}

void MainWindow::onJobListClicked()
{
    updateTableViewTicket->stop();
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

        JobListOnReceiptDialog jlord(dbConnectionString, getCurrentTDCRId().toInt(),this);
        if (jlord.exec())        
            db.commit();        
        else        
            db.rollback();                
        db.close();
    }
    updateTableViewTicket->start(DEFAULTPERIOD);
    QSqlDatabase::removeDatabase(dbConnectionString);
    refreshTicketModel(generateTicketQuery());
}

void MainWindow::on_radioButtonReady_pressed()
{
    ui->lineEditSearch->clear();
    currentStatus = Ready;
    refreshTicketModel(generateTicketQuery());
}

void MainWindow::on_radioButtonWorking_pressed()
{
    ui->lineEditSearch->clear();
    currentStatus = InWork;
    refreshTicketModel(generateTicketQuery());
}

void MainWindow::on_radioButtonClosed_pressed()
{
    ui->lineEditSearch->clear();
    currentStatus = Closed;
    refreshTicketModel(generateTicketQuery());
}

void MainWindow::on_pushButtonSearchClear_clicked()
{
    ui->lineEditSearch->clear();
    ui->radioButtonWorking->setChecked(true);
    currentStatus = InWork;
    refreshTicketModel(generateTicketQuery());
    //updateTableViewTicket->start(DEFAULTPERIOD);
}

void MainWindow::on_pushButtonSearch_clicked()
{
    updateTableViewTicket->stop();    
    refreshTicketModel(
                "select ticket.id, ticket.ticket_id, device.date_accepted, branch.branch_name, "
                "client.name, client.phone, device.name, device.serial, "
                "device.problem, device.price, ticket_guarantee.date_accepted, ticket_guarantee.id, "
                "ticket_guarantee.date_closed "
                "from ticket "
                "join client on(ticket.client_id = client.id) "
                "join device on(ticket.device_id = device.id) "
                "join branch on(device.branch_id = branch.id) "
                "left join ticket_guarantee on (ticket_guarantee.tdc_r_id = ticket.id) "
                "where cast(ticket.ticket_id AS Text) LIKE ('%"+ui->lineEditSearch->text().trimmed()+"%') "
                "or client.name LIKE ('%"+ui->lineEditSearch->text().trimmed()+"%') ORDER BY ticket.ticket_id DESC"
                );
}

void MainWindow::onShowCommentsTabClicked()
{
    ui->groupBoxFastTicketInfo->setVisible(true);
    ui->groupBoxFastTicketInfo->setVisible(true);
    ui->tabWidgetFastTicketInfo->setTabEnabled(1,true);
    ui->tabWidgetFastTicketInfo->setCurrentIndex(1);
}

void MainWindow::onAddCommentToTicketClicked()
{
    if (ui->plainTextEditComment->toPlainText().trimmed().isEmpty())
        return;
    QSqlQuery q;
    if (!SetupManager::instance()->getSqlQueryForDB(q))
        return;
    q.prepare("insert into ticket_comments(comment, tdc_relation_id, employee_id) VALUES(?,?,?) returning id,date");
    q.addBindValue(ui->plainTextEditComment->toPlainText());
    q.addBindValue(getCurrentTDCRId());
    q.addBindValue(currentEmployeeId);
    if (!q.exec())
        qDebug() << q.lastError() << q.lastQuery();
    q.next();
    QStandardItem* comment = new QStandardItem(ui->plainTextEditComment->toPlainText());
    comment->setToolTip(ui->plainTextEditComment->toPlainText());
    comment->setData(q.value(0));//comment id
    comment->setData(currentEmployeeId,Qt::UserRole + 2);
    QStandardItem* fio = new QStandardItem(currentEmployeeName);
    QStandardItem* date = new QStandardItem(q.value(1).toDateTime().toString("dd-MM-yyyy hh:mm:ss"));
    ticketComments->insertRow(0, QList<QStandardItem*>() << comment << fio << date);
    ui->plainTextEditComment->clear();
    ui->treeViewTicketComments->resizeColumnToContents(0);
    ui->treeViewTicketComments->resizeColumnToContents(1);
    ui->treeViewTicketComments->resizeColumnToContents(2);
}

void MainWindow::onRemoveCommentClicked()
{
    QSqlQuery q;
    if (!SetupManager::instance()->getSqlQueryForDB(q))
        return;
    q.prepare("delete from ticket_comments where id = ?");
    q.addBindValue(ticketComments->item(ui->treeViewTicketComments->currentIndex().row(),0)->data());
    if (!q.exec())
        qDebug() << q.lastError() << q.lastQuery();
    ticketComments->takeRow(ui->treeViewTicketComments->currentIndex().row());
}

void MainWindow::onGenerateTicketReport()
{
    qDebug() << "report generated" << ReportsHandler::openTicketReports(getCurrentTDCRId().toInt());
}

void MainWindow::onNotAGuaranteeClicked()
{
    QSqlQuery q;
    if (!SetupManager::instance()->getSqlQueryForDB(q))
        return;
    q.prepare("delete from ticket_guarantee where id = ?");
    q.addBindValue(getCurrentTicketGuaranteeId(ui->tableViewTicket->currentIndex()));

    if (!q.exec())
        qDebug() << q.lastError() << q.lastQuery();

    onCloseTicketClicked();
}

void MainWindow::onAcceptAGuaranteeClicked()
{
    QSqlQuery q;
    if (!SetupManager::instance()->getSqlQueryForDB(q))
        return;
    q.prepare("update ticket_guarantee set date_moved_to_work = ? where id = ?");
    q.addBindValue(QDateTime::currentDateTime());
    q.addBindValue(getCurrentTicketGuaranteeId(ui->tableViewTicket->currentIndex()));

    if (!q.exec())
        qDebug() << q.lastError() << q.lastQuery();

    refreshTicketModel(generateTicketQuery());
}

void MainWindow::on_tableViewTicket_doubleClicked(const QModelIndex &index)
{    
    Q_UNUSED(index);
    updateTableViewTicket->stop();
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
        ReceiptManager rm(dbConnectionString,getCurrentTDCRId().toInt(),this);
        if (rm.exec())
        {
            db.commit();
            refreshTicketModel(generateTicketQuery());
        }
        else
            db.rollback();        
        db.close();
    }
    updateTableViewTicket->start(DEFAULTPERIOD);
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
    if (!SetupManager::instance()->getSqlQueryForDB(q))
        return;

    q.prepare("select employee_FIO,job_name,job_quantity,job_price,Job_date,jot_id from JobOnTicket "
              "join Employee ON (JobOnTicket.Employee_ID=Employee.Employee_ID) where tdc_r_id=?");
    q.addBindValue(ticketModel->item(current.row(),0)->data());
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

    ui->tabWidgetFastTicketInfo->setMaximumHeight(height()*0.35);
    ui->lineEditTicketPrice->setText(QString::number(totalPrice));
    ui->groupBoxFastTicketInfo->setTitle(trUtf8("Квитанция № %0").arg(QString::number(getCurrentTicketId())));
    for (int i = 0 ; i < jobModel->columnCount(); ++i)
        ui->treeViewJobsOnTicket->resizeColumnToContents(i);

    ticketComments->clear();
    ticketComments->setHorizontalHeaderLabels(QStringList() << trUtf8("Комментарий") << trUtf8("Автор") << trUtf8("Дата"));
    q.prepare("select comment, employee_fio, date, id,ticket_comments.employee_id from ticket_comments "
              "join employee on (employee.employee_id = ticket_comments.employee_id) "
              "where tdc_relation_id = ?");
    q.addBindValue(ticketModel->item(current.row(),0)->data());
    if (!q.exec())
        qDebug() << q.lastError() << q.lastQuery();

    while(q.next())
    {
        QStandardItem* comment = new QStandardItem(q.value(0).toString());
        comment->setToolTip(q.value(0).toString());
        comment->setData(q.value(3)); //comment id;
        comment->setData(q.value(4), Qt::UserRole + 2); //employee_id
        QStandardItem* fio = new QStandardItem(q.value(1).toString());
        QStandardItem* date = new QStandardItem(q.value(2).toDateTime().toString("dd-MM-yyyy hh:mm:ss"));
        ticketComments->appendRow(QList<QStandardItem*>() << comment << fio << date);
        ui->treeViewTicketComments->resizeColumnToContents(0);
        ui->treeViewTicketComments->resizeColumnToContents(1);
        ui->treeViewTicketComments->resizeColumnToContents(2);
    }
    bool showGuarantee = false;
    if (!getCurrentTicketGuaranteeId(current).isNull())
    {
        showGuarantee = true;
        q.prepare("select problem,date_accepted,date_moved_to_work from ticket_guarantee where tdc_r_id = ?");
        q.addBindValue(getCurrentTDCRId(current));
        if (!q.exec())
            qDebug() << q.lastError() << q.lastQuery();
        q.next();
        ui->plainTextEditTicketGuaranteeProblem->setPlainText(q.value(0).toString());
        ui->dateTimeEditGuaranteeDateAccepted->setDateTime(q.value(1).toDateTime());
        if (!q.value(2).isNull())
        {
            ui->dateTimeEditGuaranteeDateAcceptedToWork->setDateTime(q.value(2).toDateTime());
            ui->labelDateAcceptedToWorkGuarantee->setVisible(true);
            ui->dateTimeEditGuaranteeDateAcceptedToWork->setVisible(true);
        }
        else
        {
            ui->labelDateAcceptedToWorkGuarantee->setVisible(false);
            ui->dateTimeEditGuaranteeDateAcceptedToWork->setVisible(false);
        }
    }
    ui->groupBoxGuarantee->setVisible(showGuarantee);
    ui->tabWidgetFastTicketInfo->setTabEnabled(TicketJobs,jobModel->rowCount() != 0 || showGuarantee);
    ui->tabWidgetFastTicketInfo->setTabEnabled(TicketComments,ticketComments->rowCount() != 0);
    ui->groupBoxFastTicketInfo->setVisible(jobModel->rowCount() != 0 || ticketComments->rowCount() != 0 || showGuarantee);
}

void MainWindow::onCustomContextMenuRequested(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);
    connect(menu, SIGNAL(aboutToHide()), menu, SLOT(deleteLater()));
    QModelIndexList qmil = ui->tableViewTicket->selectionModel()->selectedIndexes();

    QModelIndex ind = ui->tableViewTicket->indexAt(pos);
    menu->addAction(trUtf8("Добавить квитанцию"), this, SLOT(onAddTicketClicked()));
    if (ind.isValid())
    {
        if (qmil.count() == 1)
        {
            menu->addAction(trUtf8("Список работ"), this, SLOT(onJobListClicked()));
            menu->addAction(trUtf8("Посмотреть комментарии"), this, SLOT(onShowCommentsTabClicked()));
        }
        if (currentStatus != Closed)        
            menu->addAction(trUtf8("Закрыть квитанцию"), this, SLOT(onCloseTicketClicked()));
        if (currentStatus != InWork)
            menu->addAction(trUtf8("Вернуть в работу"), this, SLOT(onMoveBackToWork()));
        if (currentStatus != Ready)
            menu->addAction(trUtf8("Поместить в готовые"), this, SLOT(onMoveBackToReady()));        
        if (currentStatus == Closed)
            menu->addAction(trUtf8("Гарантия, вернуть в работу"), this, SLOT(submitGuaranteeTicket()));
    }
    menu->exec(QCursor::pos());
}

void MainWindow::onCommentsCustomContextMenuRequested(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);
    connect(menu, SIGNAL(aboutToHide()), menu, SLOT(deleteLater()));
    QModelIndex ind = ui->treeViewTicketComments->indexAt(pos);
    if (ind.isValid())
    {
        if (ticketComments->item(ind.row(),0)->data(Qt::UserRole + 2).toInt() == currentEmployeeId)
            menu->addAction(trUtf8("Удалить комментарий"), this, SLOT(onRemoveCommentClicked()));
    }
    menu->exec(QCursor::pos());
}

void MainWindow::onIsClientNotifiedClicked(const QModelIndex &index)
{
    if (index.column() != TicketNumber)
        return;
    QSqlQuery q;
    if (!SetupManager::instance()->getSqlQueryForDB(q))
        return;
    q.prepare("update ticket set client_notified = ? where id = ?");
    q.addBindValue(ticketModel->itemFromIndex(ticketProxy->mapToSource(index))->checkState() == Qt::Checked ? "TRUE" : "FALSE");
    q.addBindValue(ticketModel->item(ticketProxy->mapToSource(index).row(),TicketNumber)->data());
    q.exec();
}

void MainWindow::onMoveBackToWork()
{
    changeTicketStatus(InWork);
}

void MainWindow::onMoveBackToReady()
{
    onSetGuaranteeDone();
    changeTicketStatus(Ready);
}

void MainWindow::submitGuaranteeTicket()
{
    QSqlQuery q;
    if (!SetupManager::instance()->getSqlQueryForDB(q))
        return;
    GuaranteeOnTicketReasonWidget* gotrw = new GuaranteeOnTicketReasonWidget(this);
    QModelIndexList qmil = ui->tableViewTicket->selectionModel()->selectedIndexes();
    for (int i = 0; i < qmil.count(); ++i)
    {
        if (qmil.at(i).column() != TicketNumber)
            continue;
        gotrw->addDevice(getCurrentTDCRId(qmil.at(i)),
                         getCurrentTicketId(qmil.at(i)),
                         getCurrentTicketDeviceName(qmil.at(i)),
                         getCurrentTicketDeviceSerial(qmil.at(i)));
    }
    if (gotrw->exec())
    {
        for (int i = 0 ; i < qmil.count(); ++i)
        {
            if (qmil.at(i).column() != TicketNumber)
                continue;
            q.prepare("insert into ticket_guarantee(tdc_r_id,problem) VALUES(?,?)");
            q.addBindValue(getCurrentTDCRId(qmil.at(i)));//guarantee id
            q.addBindValue(gotrw->ticketGuaranteeText[getCurrentTDCRId(qmil.at(i)).toString()]);//guarantee text
            if (!q.exec())
                qDebug() << q.lastError() << q.lastQuery();

            q.prepare("update ticket set client_notified = 0 where id = ?");
            q.addBindValue(getCurrentTDCRId(qmil.at(i)));
            q.exec();
        }
        changeTicketStatus(InWork);
    }
}

void MainWindow::onSetGuaranteeDone()
{
    if (!getCurrentTicketGuaranteeId(ui->tableViewTicket->currentIndex()).isNull())
    {
        QSqlQuery q;
        if (!SetupManager::instance()->getSqlQueryForDB(q))
            return;
        q.prepare("update ticket_guarantee set date_closed = ? where id = ?");
        q.addBindValue(QDateTime::currentDateTime());
        q.addBindValue(getCurrentTicketGuaranteeId(ui->tableViewTicket->currentIndex()));
        if (!q.exec())
            qDebug() << q.lastError() << q.lastQuery();
    }
}

void MainWindow::onQueryLimitComboBoxIndexChanged(int)
{
    refreshTicketModel(generateTicketQuery());
}

void MainWindow::on_actionConnect_triggered()
{
    if (!connectToDb(CONNECTIONNAME))
    {
        qDebug() << "failed to connect to DB on action connect";
        return;
    }    
    refreshTicketModel(generateTicketQuery());
}

void MainWindow::on_actionDisconnect_triggered()
{
    disconnectFromDb(CONNECTIONNAME);
    ui->actionConnect->setEnabled(settingsIsNotEmpty());
}

void MainWindow::onActionBranchesTriggered()
{
    updateTableViewTicket->stop();

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

        BranchWidget bw(dbConnectionString,this);
        if (bw.exec())
        {
            db.commit();
        }
        else
            db.rollback();

        db.close();
    }
    updateTableViewTicket->start(DEFAULTPERIOD);
    QSqlDatabase::removeDatabase(dbConnectionString);
    refreshTicketModel(generateTicketQuery());
}

void MainWindow::onCloseTicketClicked()
{
    onSetGuaranteeDone();
    changeTicketStatus(Closed);
}

void MainWindow::onActionChangeUserClicked()
{
    ChangeUserDialog cud;
    if (cud.exec())    
        changeUser(cud.getUser(),cud.getPassword()) ? ui->menuLoginStatus->setIcon(QIcon(":/icons/icons/Accept-Male-User24.png")) : ui->menuLoginStatus->setIcon(QIcon(":/icons/icons/Remove-Male-User24.png"));
}

void MainWindow::onActionUserManagementClicked()
{
    QString dbConnectionString = "USERMANAMEGENT";
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
        UserManagementDialog umd(dbConnectionString, this);
        if (umd.exec())
            db.commit();
        else
            db.rollback();
        db.close();
    }
    QSqlDatabase::removeDatabase(dbConnectionString);
}

void MainWindow::onChangeUserInPopupMenu()
{
    ChangeUserDialog * cud = qobject_cast<ChangeUserDialog*>(sender());
    if (!cud)
        return;
    changeUser(cud->getUser(),cud->getPassword());
    cud->setVisible(true);
}

void MainWindow::onRejectUserInPopupMenu()
{
    ui->menuCurrentUser->hide();    
    cud->setVisible(true);
}

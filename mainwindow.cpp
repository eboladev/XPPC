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
    connect(ui->actionCloseTicket,SIGNAL(triggered()), SLOT(on_actionCloseTicket_triggered()));
    connect(ui->actionChangeUser, SIGNAL(triggered()), SLOT(onActionChangeUserClicked()));
    connect(ui->actionUserControl, SIGNAL(triggered()), SLOT(onActionUserManagementClicked()));
    connect(ui->actionAddProductCategory, SIGNAL(triggered()), SLOT(onActionCategoryProductsClicked()));
    connect(ui->tableViewTicket->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(onTableViewTicketSelectionChanged(QModelIndex,QModelIndex)));

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

    connect(ui->queryLimitComboBoxWidget, SIGNAL(limitChanged(int)), SLOT(onQueryLimitComboBoxIndexChanged(int)));
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
        return ticketModel->item(ticketProxy->mapToSource(ui->tableViewTicket->currentIndex()).row(),0)->data();
    else
        return QVariant();
}

int MainWindow::getCurrentTicketId()
{
    if (ui->tableViewTicket->currentIndex().isValid())
        return ticketModel->item(ticketProxy->mapToSource(ui->tableViewTicket->currentIndex()).row(),0)->text().toInt();
    else
        return -1;
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
    productModel->setHeaderData(0, Qt::Horizontal, trUtf8("Название"));
    productModel->setHeaderData(1, Qt::Horizontal, trUtf8("Серийный №"));
    productModel->setHeaderData(2, Qt::Horizontal, trUtf8("Цена"));
    productModel->setHeaderData(3, Qt::Horizontal, trUtf8("Количество"));
    productModel->setHeaderData(4, Qt::Horizontal, trUtf8("Гарантия"));
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
        {
            db.commit();
            onRefreshCategoryModel();
        }
        else
            db.rollback();
        db.close();
    }
    QSqlDatabase::removeDatabase(dbConnectionString);
}

QString MainWindow::generateTicketQuery()
{
    return QString("select tdc_relation.id, ticket_id, ticket.date_accepted, branch.branch_name, "
                   "client.name, client.phone, device.name, device.serial, device.problem %0 "
                   "from tdc_relation "
                   "join ticket on(tdc_relation.ticket_id = ticket.id) "
                   "join client on(tdc_relation.client_id = client.id) "
                   "join device on(tdc_relation.device_id = device.id) "
                   "join branch on(device.branch_id = branch.id) "
                   "where ticket.status = %1 "
                   "ORDER BY ticket_id DESC %2")
            .arg(currentStatus == Closed ? ", ticket.price, ticket.date_givenout" : "")
            .arg(QString::number(currentStatus))
            .arg(ui->queryLimitComboBoxWidget->getLimit() == 0 ? "" : QString("LIMIT ").append(QString::number(ui->queryLimitComboBoxWidget->getLimit())));
}

bool MainWindow::changeUser(const QString &login, const QString &password)
{
    QString passwordHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha1).toHex();
    QSqlQuery q;
    if (!SetupManager::instance()->getSqlQueryForDB(q))
        return false;
    q.prepare("select password, employee_fio, employee_id, permissions from employee where login = ?");
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

    SetupManager::instance()->setCurrentUser(login);
    SetupManager::instance()->setPermissons(permissions);
    changePermissions();
    ui->menuCurrentUser->setTitle(fio);
    return true;
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
        ticket_id->setData(q.value(0)); //tdc_relation id
        QStandardItem* date_accepted = new QStandardItem(q.value(2).toString());
        QStandardItem* branch = new QStandardItem(q.value(3).toString());
        QStandardItem* client_name = new QStandardItem(q.value(4).toString());
        QStandardItem* client_phone = new QStandardItem(q.value(5).toString());
        QStandardItem* device_name = new QStandardItem(q.value(6).toString());
        QStandardItem* device_serial = new QStandardItem(q.value(7).toString());
        QStandardItem* device_problem = new QStandardItem(q.value(8).toString());
        if (currentStatus==Closed)
        {
            QStandardItem* ticket_price = new QStandardItem(q.value(9).toString());
            QStandardItem* ticket_givenout = new QStandardItem(q.value(10).toString());
            ticketModel->appendRow(QList<QStandardItem*>() << ticket_id << date_accepted << branch << client_name << client_phone << device_name << device_serial << device_problem << ticket_price << ticket_givenout);
        }
        else
            ticketModel->appendRow(QList<QStandardItem*>() << ticket_id << date_accepted << branch << client_name << client_phone << device_name << device_serial << device_problem);
    }
    ui->tableViewTicket->resizeColumnsToContents();

    ticketModel->setHeaderData(0, Qt::Horizontal, tr("№"));
    ticketModel->setHeaderData(1, Qt::Horizontal, tr("Дата"));
    ticketModel->setHeaderData(2, Qt::Horizontal, tr("Филиал"));
    ticketModel->setHeaderData(3, Qt::Horizontal, tr("ФИО"));
    ticketModel->setHeaderData(4, Qt::Horizontal, tr("Телефон"));
    ticketModel->setHeaderData(5, Qt::Horizontal, tr("Устройство"));
    ticketModel->setHeaderData(6, Qt::Horizontal, tr("Серийный №"));
    ticketModel->setHeaderData(7, Qt::Horizontal, tr("Неисправность"));
    ui->tableViewTicket->resizeColumnToContents(3);

    if (currentStatus==Closed)
    {
        ticketModel->setHeaderData(8, Qt::Horizontal, tr("Цена"));
        ticketModel->setHeaderData(9, Qt::Horizontal, tr("Выдано"));
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
    }
    updateTableViewTicket->start(DEFAULTPERIOD);
    QSqlDatabase::removeDatabase(dbConnectionString);
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
    currentStatus = Ready;
    refreshTicketModel(generateTicketQuery());
}

void MainWindow::on_radioButtonWorking_pressed()
{
    currentStatus = InWork;
    refreshTicketModel(generateTicketQuery());
}

void MainWindow::on_radioButtonClosed_pressed()
{
    currentStatus = Closed;
    refreshTicketModel(generateTicketQuery());
}

void MainWindow::on_pushButtonSearchClear_clicked()
{
    ui->lineEdit->clear();
    ui->radioButtonWorking->setChecked(true);
    currentStatus = InWork;
    refreshTicketModel(generateTicketQuery());
    //updateTableViewTicket->start(DEFAULTPERIOD);
}

void MainWindow::on_pushButtonSearch_clicked()
{
    updateTableViewTicket->stop();
    refreshTicketModel(
                "select ticket_id,ticket_date_in, branch_name, ticket_fio,ticket_phone,ticket_device, "
                "ticket_problem from Ticket join branch on(ticket.ticket_branch = branch.id) "
                "where cast(ticket.ticket_id AS Text) LIKE ('%"+ui->lineEdit->text().trimmed()+"%') "
                "or Ticket_FIO LIKE ('%"+ui->lineEdit->text().trimmed()+"%') ORDER BY Ticket_ID DESC"
                );
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
    if (SetupManager::instance()->getSqlQueryForDB(q))
    {
        //NYI
     /*   q.prepare("select employee_FIO,job_name,job_quantity,job_price,Job_date,jot_id from JobOnTicket join Employee ON (JobOnTicket.Employee_ID=Employee.Employee_ID) where Ticket_ID=?");
        q.addBindValue(ticketModel->record(ticketProxy->mapToSource(current).row()).value(0));
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
            ui->treeViewJobsOnTicket->resizeColumnToContents(i);*/
    }
}

void MainWindow::onCustomContextMenuRequested(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);
    connect(menu, SIGNAL(aboutToHide()), menu, SLOT(deleteLater()));
    QModelIndex ind = ui->tableViewTicket->indexAt(pos);
    menu->addAction(trUtf8("Добавить квитанцию"), this, SLOT(onAddTicketClicked()));
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
    q.prepare("update ticket set status = 0 where id = ?");
    q.addBindValue(getCurrentTicketId());
    q.exec();
    refreshTicketModel(generateTicketQuery());
}

void MainWindow::onMoveBackToReady()
{
    qDebug() << getCurrentTicketId();
    QSqlQuery q;
    if (!SetupManager::instance()->getSqlQueryForDB(q))
        return;
    q.prepare("update ticket set status = 1 where id = ?");
    q.addBindValue(getCurrentTicketId());
    q.exec();
    refreshTicketModel(generateTicketQuery());
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

void MainWindow::on_actionCloseTicket_triggered()
{
    QSqlQuery q;
    if (!SetupManager::instance()->getSqlQueryForDB(q))
        return;
    q.prepare("update ticket set status = 2 where id = ?");
    q.addBindValue(getCurrentTicketId());
    q.exec();
    refreshTicketModel(generateTicketQuery());
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

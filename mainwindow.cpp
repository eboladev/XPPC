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
#include "guaranteeonticketreasonwidget.h"
#include "reportshandler.h"
#include "reportssettings.h"
#include "usersandpermissionsmanager.h"
#include "contactdeveloperdialog.h"
#include "smsgatewaysettings.h"
#include "dialogtemplate.h"
#include "smstemplatesettings.h"
#include "barcodescannersettingsdialog.h"
#include "jobitemmodel.h"
#include "employeesalarycalculationdialog.h"
#include "employeepenaltyandbonusesdialog.h"

#include "smsmanager.h"

#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QWidgetAction>
#include <QStandardItem>
#include <QMessageBox>

const QString CONNECTIONNAME = "XP";
const int DEFAULTPERIOD = 5000;
const int STATUSBARTIMEOUT = 10000;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initAccessManager();
    connectEstablished = false;
    jobModel = new JobItemModel("XP",this);
    ui->treeViewJobsOnTicket->setModel(jobModel);
    ui->groupBoxFastTicketInfo->setVisible(false);
#ifdef RELEASE
    ui->tabWidget->setCurrentIndex(0);
#endif
    ticketModel = new QStandardItemModel(this);
    ticketProxy = new QSortFilterProxyModel(this);
    ticketProxy->setSourceModel(ticketModel);
    ticketProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->tableViewTicket->setModel(ticketProxy);
    ui->tableViewTicket->setContextMenuPolicy(Qt::CustomContextMenu);
    updateTableViewTicket = new QTimer(this);
    currentStatus = InWork;

    connect(ui->tableViewTicket, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(onCustomContextMenuRequested(QPoint)));
    connect(ui->actionReportSettings, SIGNAL(triggered()),
            SLOT(onActionReportSettignsClicked()));
    connect(ui->actionOnAddReceiptClicked, SIGNAL(triggered()),
            SLOT(onAddTicketClicked()));
    connect(ui->actionOnJobListClicked,SIGNAL(triggered()),
            SLOT(onJobListClicked()));
    connect(ui->actionExitMenuClicked,SIGNAL(triggered()),
            SLOT(close()));
    connect(ui->actionSettingsMenuClicked,SIGNAL(triggered()),
            SLOT(onDBSettingsClicked()));
    connect(ui->actionBranchTriggered,SIGNAL(triggered()),
            SLOT(onActionBranchesTriggered()));
    connect(ui->actionCloseTicket,SIGNAL(triggered()),
            SLOT(onCloseTicketClicked()));
    connect(ui->actionChangeUser, SIGNAL(triggered()),
            SLOT(onActionChangeUserClicked()));
    connect(ui->actionUserControl, SIGNAL(triggered()),
            SLOT(onActionUserManagementClicked()));
    connect(ui->actionAddProductCategory, SIGNAL(triggered()),
            SLOT(onActionCategoryProductsClicked()));
    connect(ui->actionPrintTicket, SIGNAL(triggered()),
            SLOT(onGenerateTicketReport()));
    connect(ui->actionJobOnTicketPrint, SIGNAL(triggered()),
            SLOT(onGenerateJobListReport()));
    connect(ui->actionCashReceiptPrint, SIGNAL(triggered()),
            SLOT(onGenerateCashReceiptReport()));
    connect(ui->actionSMSGatewaysSettings, SIGNAL(triggered()),
            SLOT(onActionSMSGatewaysSettingsClicked()));
    connect(ui->action_Qt, SIGNAL(triggered()),
            qApp, SLOT(aboutQt()));
    connect(ui->actionDeveloperContact, SIGNAL(triggered()),
            this, SLOT(onActionDeveloperContactClicked()));
    connect(ui->tableViewTicket->selectionModel(),
            SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(onTableViewTicketSelectionChanged(QModelIndex,QModelIndex)));
    connect(ui->tableViewTicket, SIGNAL(clicked(QModelIndex)),
            SLOT(onIsClientNotifiedClicked(QModelIndex)));
    connect(ui->pushButtonSendSms, SIGNAL(clicked()),
            this, SLOT(onSendTicketNotifySmsClicked()));
    connect(ui->actionSmsTemplateSettings, SIGNAL(triggered()),
            this, SLOT(onActionSmsTemplateSettingsClicked()));
    connect(ui->plainTextEditSmsText, SIGNAL(textChanged()),
            this, SLOT(onTicketNotificationSmsTextChanged()));
    connect(ui->lineEditTicketPrice, SIGNAL(textChanged(QString)),
            this, SLOT(onTicketPriceChanged(QString)));
    connect(ui->actionBarcodeScannerSettings, SIGNAL(triggered()),
            this, SLOT(onBarcodeScannerSettingsClicked()));
    connect(ui->actionEmployeeSalaryCalculation, SIGNAL(triggered()),
            this, SLOT(onEmployeeSalaryCalculationDialogClicked()));
    connect(ui->actionPenaltyAndBonuses, SIGNAL(triggered()),
            this, SLOT(onActionPenaltyAndBonusesClicked()));

    connect(smsManager, SIGNAL(gwBalance(double)),
            this, SLOT(onBalance(double)));
    connect(smsManager, SIGNAL(smsDelivered(int)),
            this, SLOT(onSmsDelivered(int)));
    connect(smsManager, SIGNAL(smsSended(int)),
            this, SLOT(onSmsSended(int)));
    connect(smsManager, SIGNAL(smsInProcess(int)),
            this, SLOT(onSmsInProcess(int)));
    connect(smsManager, SIGNAL(smsNonDelivered(int)),
            this, SLOT(onSmsNonDelivered(int)));

    ui->actionOnJobListClicked->setEnabled(false);
    ui->actionCloseTicket->setEnabled(false);
    ui->actionConnect->setEnabled(checkDbSettings());

    on_actionConnect_triggered();

    changePermissions();

    //do NOT delete it... yet.
    //connect(updateTableViewTicket,SIGNAL(timeout()),this,SLOT(refreshTicketModel()));
    ticketComments = new QStandardItemModel(this);
    ui->treeViewTicketComments->setModel(ticketComments);
    ui->treeViewTicketComments->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeViewTicketComments->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->treeViewTicketComments, SIGNAL(customContextMenuRequested(QPoint)), SLOT(onCommentsCustomContextMenuRequested(QPoint)));

    connect(ui->pushButtonNotAGuarantee, SIGNAL(clicked()), SLOT(onNotAGuaranteeClicked()));
    connect(ui->pushButtonAcceptToWorkGuarantee, SIGNAL(clicked()), SLOT(onAcceptAGuaranteeClicked()));
    connect(ui->pushButtonAddComment, SIGNAL(clicked()), SLOT(onAddCommentToTicketClicked()));

#ifdef DEBUG
    //changeUser(cud->getUser(),cud->getPassword());
#endif

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
    if (ui->tabWidget->currentIndex() == ProductsTab)
        onRefreshCategoryModel();

    connect(ui->queryLimitComboBoxWidget, SIGNAL(limitChanged(int)), SLOT(onQueryLimitComboBoxIndexChanged(int)));
#ifdef Q_WS_MAC
    ui->menuLoginStatus->setVisible(false);
    ui->menuLoginStatus->setHidden(true);
#endif
#ifdef RELEASE
    ui->menuShowcase->setEnabled(false);
    ui->tabWidget->setTabEnabled(1,false);
#endif
    // updateTableViewTicket->start(DEFAULTPERIOD);
    qApp->installEventFilter(this);
    setWindowTitle(trUtf8("Управление СЦ %0").arg(qApp->applicationVersion()));
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
    if (status != Closed)
    {
        QModelIndexList qmil = ui->tableViewTicket->selectionModel()->selectedIndexes();
        for (int i = 0 ; i < qmil.count(); ++i)
        {
            if (qmil.at(i).column() != TicketNumber)
                continue;
            q.prepare(QString("update device set status = ? "
                              "where device.id = (select device_id from ticket where id = ?)"));
            q.addBindValue(status);
            q.addBindValue(getCurrentTDCRId(qmil.at(i)));
            if (!q.exec())
                qDebug() << q.lastError() << q.lastQuery();
        }
    }
    else
    {
        q.prepare(QString("update device set status = ? , date_givenout = ?, price = ? "
                          "where device.id = (select device_id from ticket where id = ?)"));
        q.addBindValue(status);
        q.addBindValue(QDate::currentDate().toString("dd.MM.yyyy"));
        q.addBindValue(getCurrentTicketDevicePrice());
        q.addBindValue(getCurrentTDCRId(ui->tableViewTicket->currentIndex()));
        if (!q.exec())
            qDebug() << q.lastError() << q.lastQuery();
    }

    qDebug() << Q_FUNC_INFO;
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

QString MainWindow::getCurrentTicketDevicePrice()
{
    return ui->lineEditTicketPrice->text();
}

QString MainWindow::getCurrentTicketDeviceSerial(const QModelIndex &index)
{
    if (index.isValid())
        return ticketModel->item(ticketProxy->mapToSource(index).row(),TicketDeviceSerial)->text();
    else
        return "";
}

QString MainWindow::getCurrentTicketPhones(const QModelIndex &index)
{
    if (index.isValid())
        return ticketModel->item(ticketProxy->mapToSource(index).row(),TicketClientPhone)->text();
    else
        return "";
}

QString MainWindow::getCurrentTicketData(const TicketColumns &column, const TicketRoles &role)
{
    if (ui->tableViewTicket->currentIndex().isValid())
        return ticketModel->item(ticketProxy->mapToSource(ui->tableViewTicket->currentIndex()).row(),column)->data(role).toString();
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

void MainWindow::updateSmsWidget(const int &id, const int &ticket_id)
{
    //  if (getCurrentTicketId() == ticket_id)
    {
        QSqlQuery q;
        if (!setupManager->getSqlQueryForDB(q))
            return;
        QString query = QString("select status, last_status_time from sended_sms where %0 = ? order by id desc")
                .arg(ticket_id > 0 ? "ticket_id" : "id");
        q.prepare(query);
        q.addBindValue(ticket_id > 0 ? ticket_id : id);
        q.exec();
        if (q.next())
        {
            QString text;
            switch(q.value(0).toInt())
            {
            case XPPC::InProcess:
                text = trUtf8("Обрабатывается");
                break;
            case XPPC::Sended:
                text = trUtf8("Отправлено");
                break;
            case XPPC::Delivered:
                text = trUtf8("Доставлено");
                break;
            case XPPC::InQueue:
                text = trUtf8("В очереди");
                break;
            case XPPC::Aborted:
                text = trUtf8("Отменено");
                break;
            case XPPC::NonDelivered:
                text = trUtf8("Не доставлено");
                break;
            case XPPC::NonSended:
                text = trUtf8("Не отправлено");
                break;
            case XPPC::NotFound:
                text = trUtf8("Не найдено");
                break;
            default:
                text = trUtf8("Статус");
                break;
            }
            ui->labelStatus->setText(text);
            ui->dateTimeEditLastStatusTime->setDateTime(q.value(1).toDateTime());
            ui->labelStatus->setVisible(true);
            ui->dateTimeEditLastStatusTime->setVisible(true);
        }
        else
        {
            ui->labelStatus->setVisible(false);
            ui->dateTimeEditLastStatusTime->setVisible(false);
        }
    }
}

QString MainWindow::genTicketNotifySms()
{
    QString templateText;
    QSettings s;
    templateText = s.value("smsTemplates/ticket").toString();
    templateText.replace("%ticket_number%",QString::number(getCurrentTicketId()));
    templateText.replace("%date_accepted%",getCurrentTicketData(TicketDate));
    templateText.replace("%client_name%",getCurrentTicketData(TicketDate));
    templateText.replace("%client_phone%",getCurrentTicketData(TicketClientPhone));
    templateText.replace("%device_name%",getCurrentTicketData(TicketClientName));
    templateText.replace("%device_serial%",getCurrentTicketData(TicketDeviceSerial));
    templateText.replace("%device_malfunction%",getCurrentTicketData(TicketProblem));
    templateText.replace("%device_malfunction%",getCurrentTicketData(TicketProblem));
    templateText.replace("%branch_description%",getCurrentTicketData(TicketBranch,BranchDesc));
    templateText.replace("%current_price%",ui->lineEditTicketPrice->text());
    /*templateText.replace("%ticket_price%",getCurrentTicketData(TicketPrice));
    templateText.replace("%ticket_givenout%",getCurrentTicketData(TicketGivenOut));*/
    return templateText;
}

void MainWindow::sb(QString text)
{
    ui->statusBar->showMessage(text, STATUSBARTIMEOUT);
}

void MainWindow::changePermissions()
{
    if (accessManager->getCurrentUser().isEmpty())
    {
        ui->actionChangeUser->setIcon(QIcon(":/icons/icons/Remove-Male-User24.png"));
        ui->menuCurrentUser->setTitle(trUtf8("Вход не выполнен"));
    }
    else
        ui->actionChangeUser->setIcon(QIcon(":/icons/icons/Accept-Male-User24.png"));

    bool permissions = accessManager->isUserLoggedIn();// || accessManager->getPermissions();
    qDebug() << accessManager->getCurrentUser() << Q_FUNC_INFO << permissions << accessManager->getPermissions();
    ui->tabTickets->setEnabled(permissions);
    ui->tabShowcase->setEnabled(permissions);
    ui->menuTicket->setEnabled(permissions);
    ui->menuShowcase->setEnabled(permissions);
    ui->menuSettings->setEnabled(permissions);
    ui->menuEmployee->setEnabled(permissions);
    ui->actionDeveloperContact->setEnabled(permissions);
    ui->tabWidget->setEnabled(permissions);
    ui->tabWidgetFastTicketInfo->setEnabled(permissions);
    ui->treeViewJobsOnTicket->setEnabled(permissions);
    foreach (QAction* act,ui->menuTicket->actions())
        act->setEnabled(permissions);
    foreach (QAction* act,ui->menuShowcase->actions())
        act->setEnabled(permissions);
    ui->actionOnAddReceiptClicked->setEnabled(accessManager->isCanAddTicket());
    ui->actionCloseTicket->setEnabled(accessManager->isCanCloseTicket());
    ui->actionEmployeeSalaryCalculation->setEnabled(accessManager->isCanEditSalary());
    ui->actionPenaltyAndBonuses->setEnabled(accessManager->isCanEditSalary());
    ui->actionOnJobListClicked->setEnabled((currentStatus == InWork && accessManager->isUserLoggedIn()) || accessManager->isCanEditJobList());
}

void MainWindow::onUserLogOut()
{
    ui->actionChangeUser->setIcon(QIcon(":/icons/icons/Remove-Male-User24.png"));
    changePermissions();
}

void MainWindow::onUserLogIn()
{
    qDebug() << Q_FUNC_INFO;
    ui->actionChangeUser->setIcon(QIcon(":/icons/icons/Accept-Male-User24.png"));
    changePermissions();
    if (ui->tabWidget->currentIndex() == 0)
        refreshTicketModel(generateTicketQuery());
    ui->lineEditSearch->setFocus();
}

void MainWindow::onFailedToLogin(const QString & error)
{
    QMessageBox::information(this, trUtf8("Ошибка!"), error);
}

void MainWindow::serUserDisplayName()
{
    ui->menuCurrentUser->setTitle(accessManager->getCurrentUserDisplayName());
}

void MainWindow::onActionDeveloperContactClicked()
{
    ContactDeveloperDialog* cdd = new ContactDeveloperDialog(this);
    cdd->exec();
}

void MainWindow::onActionSmsTemplateSettingsClicked()
{
    SmsTemplateSettings* sts = new SmsTemplateSettings(this);
    sts->exec();
}

void MainWindow::onActionPenaltyAndBonusesClicked()
{
    DialogTemplate::executeDialogWithDbCheck<EmployeePenaltyAndBonusesDialog>(this);
}

void MainWindow::onBarcodeScannerSettingsClicked()
{
    BarcodeScannerSettingsDialog* bssd = new BarcodeScannerSettingsDialog(this);
    bssd->exec();
}

void MainWindow::onEmployeeSalaryCalculationDialogClicked()
{
    DialogTemplate::executeDialogWithDbCheck<EmployeeSalaryCalculationDialog>(this);
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
    DialogTemplate::executeDialogWithDbCheck<ProductCategoryManager>(this);
    onRefreshCategoryModel();
}

void MainWindow::onTicketNotificationSmsTextChanged()
{
    ui->lineEditCharCount->setText(QString::number(ui->plainTextEditSmsText->toPlainText().count()));
}

void MainWindow::onTicketPriceChanged(QString)
{
    ui->plainTextEditSmsText->setPlainText(genTicketNotifySms());
}

QString MainWindow::generateTicketQuery(const bool &search)
{
    if (search)
        return QString("SELECT "
                       "ticket.id, " //0
                       "ticket.ticket_id, "  //1
                       "device.date_accepted, " //2
                       "branch.branch_name, " //3
                       "client.name, " //4
                       "client.phone, " //5
                       "device.name, " //6
                       "device.serial, " //7
                       "device.problem, " //8
                       "ticket.client_notified, " //9
                       "ticket_guarantee.date_accepted, " //10
                       "ticket_guarantee.id, " //11
                       "ticket_guarantee.date_closed, " //12
                       "branch.branch_description, " //13
                       "device.price, " //14
                       "device.date_givenout " //15
                       "FROM "
                       "client_ticket "
                       "INNER JOIN client ON (client_ticket.client_id = client.id) "
                       "INNER JOIN ticket ON (ticket.ticket_id = client_ticket.id) "
                       "left JOIN ticket_guarantee ON (ticket.id = ticket_guarantee.tdc_r_id) "
                       "INNER JOIN device ON (device.id = ticket.device_id) "
                       "INNER JOIN branch ON (device.branch_id = branch.id) "
                       "where cast(ticket.ticket_id AS Text) LIKE ('%"+ui->lineEditSearch->text().trimmed()+"%') "
                       "or client.name LIKE ('%"+ui->lineEditSearch->text().trimmed()+"%') ORDER BY ticket.ticket_id DESC"
                       );
    else
        return QString("SELECT "
                       "ticket.id, " //0
                       "ticket.ticket_id, "  //1
                       "device.date_accepted, " //2
                       "branch.branch_name, " //3
                       "client.name, " //4
                       "client.phone, " //5
                       "device.name, " //6
                       "device.serial, " //7
                       "device.problem, " //8
                       "ticket.client_notified, " //9
                       "ticket_guarantee.date_accepted, " //10
                       "ticket_guarantee.id, " //11
                       "ticket_guarantee.date_closed, " //12
                       "branch.branch_description " //13
                       "%0 " //14,15
                       "FROM "
                       "client_ticket "
                       "INNER JOIN client ON (client_ticket.client_id = client.id) "
                       "INNER JOIN ticket ON (ticket.ticket_id = client_ticket.id) "
                       "left JOIN ticket_guarantee ON (ticket.id = ticket_guarantee.tdc_r_id) "
                       "INNER JOIN device ON (device.id = ticket.device_id) "
                       "INNER JOIN branch ON (device.branch_id = branch.id) "
                       "WHERE "
                       "device.status = %1 "
                       "ORDER BY "
                       "ticket.id DESC %2")
                .arg(currentStatus == Closed ? ",device.price, device.date_givenout " : QString())
                .arg(QString::number(currentStatus))
                .arg(ui->queryLimitComboBoxWidget->getLimit() == 0 ? "" : QString("LIMIT ")
                                                                     .append(QString::number(ui->queryLimitComboBoxWidget->getLimit())));
}

void MainWindow::initAccessManager()
{
    connect(accessManager, SIGNAL(userLogIn()), this, SLOT(onUserLogIn()));
    connect(accessManager, SIGNAL(userLogOut()), this, SLOT(onUserLogOut()));
    connect(accessManager, SIGNAL(changeUserDisplayName()),this, SLOT(serUserDisplayName()));
    connect(accessManager, SIGNAL(failedToLogin(QString)),this, SLOT(onFailedToLogin(QString)));
    connect(this, SIGNAL(disconnectedFromDb()), accessManager, SLOT(onUserLogOut()));
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
        QString tdcrId = q.value(0).toString();
        QString ticketId = q.value(1).toString();
        QString dateAccepted = q.value(2).toDate().toString("dd-MM-yyyy");
        QString branchName = q.value(3).toString();
        QString clientName = q.value(4).toString();
        QString clientPhone = q.value(5).toString();
        QString deviceName = q.value(6).toString();
        QString deviceSerial = q.value(7).toString();
        QString deviceProblem = q.value(8).toString();
        bool clientNotified = q.value(9).toBool();
        QString ticketGuaranteeAcceptDate = q.value(10).toString();
        QString ticketGuaranteeId = q.value(11).toString();
        QString ticketGuaranteeCloseDate = q.value(12).toString();
        QString branchDesc = q.value(13).toString();


        QStandardItem* ticket_id = new QStandardItem(ticketId);
        ticket_id->setData(tdcrId,TDC_Relation); //ticket id
        ticket_id->setCheckable(true);
        ticket_id->setCheckState(clientNotified ? Qt::Checked : Qt::Unchecked);
        ticket_id->setToolTip(trUtf8("Клиенту %0").arg(clientNotified ? trUtf8("отзвонились")
                                                                      : trUtf8("не отзванивались")));
        QStandardItem* date_accepted = new QStandardItem(dateAccepted);
        date_accepted->setToolTip(date_accepted->text());
        QStandardItem* branch = new QStandardItem(branchName);
        branch->setToolTip(branchName);
        QStandardItem* client_name = new QStandardItem(clientName);
        client_name->setToolTip(clientName);
        QStandardItem* client_phone = new QStandardItem(clientPhone);
        client_phone->setToolTip(clientPhone);
        QStandardItem* device_name = new QStandardItem(deviceName);
        device_name->setToolTip(deviceName);
        QStandardItem* device_serial = new QStandardItem(deviceSerial);
        device_serial->setToolTip(deviceSerial);
        QStandardItem* device_problem = new QStandardItem(deviceProblem);
        device_problem->setToolTip(QString("<table><tr><td>")
                                   .append(deviceProblem)
                                   .append("</td></td></table>"));
        branch->setData(branchDesc,BranchDesc);
        if (!ticketGuaranteeAcceptDate.isEmpty() && ticketGuaranteeCloseDate.isEmpty())
        {
            ticket_id->setIcon(QIcon(":/icons/icons/emblem-important_9062.png"));
            ticket_id->setData(ticketGuaranteeId,GuaranteeId);
        }

        if (currentStatus==Closed)
        {
            QString devicePrice = q.value(14).toString();
            QString deviceGiveOutDate = q.value(15).toDate().toString("dd-MM-yyyy");

            QStandardItem* ticket_price = new QStandardItem(devicePrice);
            ticket_price->setToolTip(devicePrice);

            QStandardItem* ticket_givenout = new QStandardItem(deviceGiveOutDate);
            ticket_givenout->setToolTip(ticket_givenout->text());

            ticketModel->appendRow(QList<QStandardItem*>() << ticket_id << date_accepted
                                   << branch << client_name << client_phone
                                   << device_name << device_serial << device_problem
                                   << ticket_price << ticket_givenout);
        }
        else
            ticketModel->appendRow(QList<QStandardItem*>() << ticket_id << date_accepted << branch
                                   << client_name << client_phone << device_name
                                   << device_serial << device_problem);
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
            !SetupManager::instance()->getDbUserName().isEmpty());
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
        qDebug() << "trying to remove db" << dbConnectionName;

        ticketModel->clear();
        emit disconnectedFromDb();
        QSqlDatabase::database(dbConnectionName).close();
        QSqlDatabase::removeDatabase(dbConnectionName);
        ui->actionChangeUser->setEnabled(false);
        changeUserDialog->onLogout();
        qDebug() << QSqlDatabase::connectionNames();
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

void MainWindow::onDBSettingsClicked()
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
    if (DialogTemplate::executeDialogWithDbCheck<ReceiptManager>(-1,this))
    {
        ui->radioButtonWorking->setChecked(true);
        currentStatus = InWork;
        refreshTicketModel(generateTicketQuery());        
    }
    updateTableViewTicket->start(DEFAULTPERIOD);
}

void MainWindow::onJobListClicked()
{
    updateTableViewTicket->stop();
    QModelIndex current = ui->tableViewTicket->currentIndex();
    DialogTemplate::executeDialogWithDbCheck<JobListOnReceiptDialog>(getCurrentTDCRId().toInt(),getCurrentTicketId(),this);
    updateTableViewTicket->start(DEFAULTPERIOD);
    onTableViewTicketSelectionChanged(current,QModelIndex());
}

void MainWindow::on_radioButtonReady_pressed()
{
    ui->lineEditSearch->clear();
    currentStatus = Ready;
    ui->actionOnJobListClicked->setEnabled(accessManager->isCanEditJobList());
    qDebug() << Q_FUNC_INFO;
    refreshTicketModel(generateTicketQuery());
}

void MainWindow::on_radioButtonWorking_pressed()
{
    ui->lineEditSearch->clear();
    currentStatus = InWork;
    ui->actionOnJobListClicked->setEnabled(accessManager->isUserLoggedIn());
    refreshTicketModel(generateTicketQuery());
}

void MainWindow::on_radioButtonClosed_pressed()
{
    ui->lineEditSearch->clear();
    currentStatus = Closed;
    ui->actionOnJobListClicked->setEnabled(accessManager->isCanEditJobList());
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
    if (!ui->lineEditSearch->text().isEmpty())
    {
        updateTableViewTicket->stop();
        // currentStatus = Closed; //temp workaround
        refreshTicketModel(generateTicketQuery(!ui->lineEditSearch->text().isEmpty()));
    }
    else
        on_pushButtonSearchClear_clicked();
}

void MainWindow::onShowCommentsTabClicked()
{
    ui->groupBoxFastTicketInfo->setVisible(true);
    ui->tabWidgetFastTicketInfo->setTabEnabled(1,true);
    ui->tabWidgetFastTicketInfo->setCurrentIndex(1);
    ui->tabWidgetFastTicketInfo->setEnabled(true);
    ui->groupBoxFastTicketInfo->setEnabled(true);
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
    q.addBindValue(accessManager->getCurrentUserId());
    if (!q.exec())
        qDebug() << q.lastError() << q.lastQuery();
    q.next();
    QStandardItem* comment = new QStandardItem(ui->plainTextEditComment->toPlainText());
    comment->setToolTip(ui->plainTextEditComment->toPlainText());
    comment->setData(q.value(0));//comment id
    comment->setData(accessManager->getCurrentUserId(),Qt::UserRole + 2);
    QStandardItem* fio = new QStandardItem(accessManager->getCurrentUserDisplayName());
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
    qDebug() << "report generated" << ReportsHandler::openReport(getCurrentTDCRId().toInt(),TicketReport);
}

void MainWindow::onGenerateJobListReport()
{
    qDebug() << "report generated" << ReportsHandler::openReport(getCurrentTDCRId().toInt(),JobListReport);
}

void MainWindow::onGenerateCashReceiptReport()
{
    qDebug() << "report generated" << ReportsHandler::openReport(getCurrentTDCRId().toInt(),CashReceiptReport);
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
    if (DialogTemplate::executeDialogWithDbCheck<ReceiptManager>(getCurrentTDCRId().toInt(),this))
        refreshTicketModel(generateTicketQuery());
    updateTableViewTicket->start(DEFAULTPERIOD);
}

void MainWindow::onTableViewTicketSelectionChanged(QModelIndex current, QModelIndex previous)
{
    Q_UNUSED(previous);
    if (!current.isValid())
        return;
    if (currentStatus != InWork)
        ui->actionOnJobListClicked->setEnabled(current.isValid() && accessManager->isCanEditJobList());
    else
        ui->actionOnJobListClicked->setEnabled(current.isValid());

    ui->actionCloseTicket->setEnabled(current.isValid() && accessManager->isCanCloseTicket());

    int totalPrice = jobModel->getJobs(getCurrentTDCRId());

    //ui->tabWidgetFastTicketInfo->setMaximumHeight(height()*0.35);
    ui->lineEditTicketPrice->setText(QString::number(totalPrice));
    ui->groupBoxFastTicketInfo->setTitle(trUtf8("Квитанция № %0").arg(QString::number(getCurrentTicketId())));
    for (int i = 0 ; i < jobModel->columnCount(); ++i)
        ui->treeViewJobsOnTicket->resizeColumnToContents(i);

    ticketComments->clear();
    ticketComments->setHorizontalHeaderLabels(QStringList() << trUtf8("Комментарий") << trUtf8("Автор") << trUtf8("Дата"));

    QSqlQuery q;
    if (!SetupManager::instance()->getSqlQueryForDB(q))
        return;
    q.prepare("select comment, employee_fio, date, id,ticket_comments.employee_id from ticket_comments "
              "join employee on (employee.employee_id = ticket_comments.employee_id) "
              "where tdc_relation_id = ? order by date DESC");
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
            ui->groupBoxGuaranteeOperations->setVisible(false);
        }
        else
        {
            ui->labelDateAcceptedToWorkGuarantee->setVisible(false);
            ui->dateTimeEditGuaranteeDateAcceptedToWork->setVisible(false);
            ui->groupBoxGuaranteeOperations->setVisible(true);
        }
    }
    ui->groupBoxGuarantee->setVisible(showGuarantee);
    //qDebug() << currentStatus << Q_FUNC_INFO;

    ui->groupBoxSms->setVisible(currentStatus == Ready);
    if (currentStatus == Ready)
    {
        QString phones = getCurrentTicketPhones(ui->tableViewTicket->currentIndex());
        qDebug() << phones << Q_FUNC_INFO;
        ui->lineEditPhones->setText(phones);

        updateSmsWidget(-1,getCurrentTicketId());
        ui->pushButtonSendSms->setEnabled(ui->lineEditBalance->text().toDouble() >= 0.15);
        ui->plainTextEditSmsText->setPlainText(genTicketNotifySms());
    }

    ui->tabWidgetFastTicketInfo->setTabEnabled(TicketJobs,jobModel->rowCount() != 0 || showGuarantee);
    //    ui->tabWidgetFastTicketInfo->setTabEnabled(TicketComments,ticketComments->rowCount() != 0);
    ui->groupBoxFastTicketInfo->setVisible(jobModel->rowCount() != 0 || ticketComments->rowCount() != 0 || showGuarantee);
    ui->groupBoxFastTicketInfo->setEnabled(jobModel->rowCount() != 0 || ticketComments->rowCount() != 0 || showGuarantee);
    ui->tabWidgetFastTicketInfo->setCurrentIndex(ui->tabWidgetFastTicketInfo->isTabEnabled(0) ? 0 : 1);
}

void MainWindow::onCustomContextMenuRequested(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);
    connect(menu, SIGNAL(aboutToHide()), menu, SLOT(deleteLater()));
    QModelIndexList qmil = ui->tableViewTicket->selectionModel()->selectedRows(TicketNumber);
    QModelIndex ind = ui->tableViewTicket->indexAt(pos);
    if (accessManager->isCanAddTicket())
        menu->addAction(trUtf8("Добавить квитанцию"), this, SLOT(onAddTicketClicked()));
    if (ind.isValid())
    {
        if (qmil.count() == 1)
        {
            if ((currentStatus == InWork && accessManager->isUserLoggedIn()) || accessManager->isCanEditJobList())
                menu->addAction(trUtf8("Список работ"), this, SLOT(onJobListClicked()));
            menu->addAction(trUtf8("Комментарии"), this, SLOT(onShowCommentsTabClicked()));
        }
        if (currentStatus != Closed && accessManager->isCanCloseTicket())
            menu->addAction(trUtf8("Закрыть квитанцию"), this, SLOT(onCloseTicketClicked()));
        if (currentStatus == Closed && accessManager->isCanEditClosedTickets())
        {
            menu->addAction(trUtf8("Вернуть в работу"), this, SLOT(onMoveBackToWork()));
            menu->addAction(trUtf8("Поместить в готовые"), this, SLOT(onMoveBackToReady()));
        } else if (currentStatus != Closed) {
            if (currentStatus != InWork && accessManager->isCanMoveToWork())
                menu->addAction(trUtf8("Вернуть в работу"), this, SLOT(onMoveBackToWork()));
            if (currentStatus != Ready && accessManager->isCanMoveToReady())
                menu->addAction(trUtf8("Поместить в готовые"), this, SLOT(onMoveBackToReady()));
        }
        if (currentStatus == Closed && accessManager->isCanGuaranteeReturn())
            menu->addAction(trUtf8("Гарантия, вернуть в работу"), this, SLOT(submitGuaranteeTicket()));
        QMenu *menuPrint = new QMenu(trUtf8("Печать"),menu);
        menu->addMenu(menuPrint);
        menuPrint->addAction(trUtf8("Квитанция"), this, SLOT(onGenerateTicketReport()));
        menuPrint->addAction(trUtf8("Список работ"), this, SLOT(onGenerateJobListReport()));
        if (currentStatus == Closed)
            menuPrint->addAction(trUtf8("ПКО"), this, SLOT(onGenerateCashReceiptReport()));
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
        if (ticketComments->item(ind.row(),0)->data(Qt::UserRole + 2).toInt() == accessManager->getCurrentUserId())
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
    q.prepare("update ticket set client_notified = ? where ticket_id = ?");
    q.addBindValue(ticketModel->itemFromIndex(ticketProxy->mapToSource(index))->checkState() == Qt::Checked ? "TRUE" : "FALSE");
    q.addBindValue(ticketModel->item(ticketProxy->mapToSource(index).row(),TicketNumber)->data());
    q.exec();
    if (ticketModel->itemFromIndex(ticketProxy->mapToSource(index))->checkState() == Qt::Checked)
        ticketModel->itemFromIndex(ticketProxy->mapToSource(index))->setToolTip("Клиенту отзвонились.");
    else
        ticketModel->itemFromIndex(ticketProxy->mapToSource(index))->setToolTip("Клиенту не отзванивались.");
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

void MainWindow::onSendTicketNotifySmsClicked()
{
    smsManager->sendSms(ui->plainTextEditSmsText->toPlainText(),QStringList() << ui->lineEditPhones->text().split(";"),getCurrentTicketId());
    /* ui->labelStatus->setText(trUtf8("Обрабатывается"));
    ui->dateTimeEditLastStatusTime->setDateTime(QDateTime::currentDateTime());*/
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

void MainWindow::onSmsSended(const int &ticket_id)
{
    qDebug() << Q_FUNC_INFO << ticket_id;
    updateSmsWidget(ticket_id);
}

void MainWindow::onSmsDelivered(const int &ticket_id)
{
    qDebug() << Q_FUNC_INFO << ticket_id;
    updateSmsWidget(ticket_id);
    {
        QSqlQuery q;
        if (!setupManager->getSqlQueryForDB(q))
            return;
        q.prepare("select ticket_id from sended_sms where id = ?");
        q.addBindValue(ticket_id);
        q.exec();
        q.next();
        int id = q.value(0).toInt();
        qDebug() << id;
        q.prepare("update ticket set client_notified = ? where ticket_id = ?");
        q.addBindValue("TRUE");
        q.addBindValue(id);
        if (!q.exec())
            qDebug() << q.lastError() << q.lastQuery();
    }
}

void MainWindow::onSmsInProcess(const int &id)
{
    qDebug() << Q_FUNC_INFO << id;
    updateSmsWidget(id);
}

void MainWindow::onSmsNonDelivered(const int &id)
{
    qDebug() << Q_FUNC_INFO << id;
    updateSmsWidget(id);
}

void MainWindow::onBalance(const double &currency)
{
    ui->lineEditBalance->setText(QString::number(currency));
}

void MainWindow::on_actionConnect_triggered()
{
    connectEstablished = connectToDb(CONNECTIONNAME);
    qDebug() << connectEstablished << Q_FUNC_INFO;
    ui->actionChangeUser->setEnabled(connectEstablished);
}

void MainWindow::on_actionDisconnect_triggered()
{
    connectEstablished = false;
    disconnectFromDb(CONNECTIONNAME);
    ui->actionConnect->setEnabled(settingsIsNotEmpty());
}

void MainWindow::onActionBranchesTriggered()
{
    updateTableViewTicket->stop();
    DialogTemplate::executeDialogWithDbCheck<BranchWidget>(this);
    updateTableViewTicket->start(DEFAULTPERIOD);
    refreshTicketModel(generateTicketQuery());
}

void MainWindow::onCloseTicketClicked()
{
    onSetGuaranteeDone();
    changeTicketStatus(Closed);
}

void MainWindow::onActionChangeUserClicked()
{
    accessManager->showChangeUserDialog();
}

void MainWindow::onActionUserManagementClicked()
{
    DialogTemplate::executeDialogWithDbCheck<UserManagementDialog>(this);
}

void MainWindow::onActionReportSettignsClicked()
{
    ReportsSettings rs;
    rs.exec();
}

void MainWindow::onActionSMSGatewaysSettingsClicked()
{
    SmsGatewaySettings* sgs = new SmsGatewaySettings(this);
    sgs->exec();
}

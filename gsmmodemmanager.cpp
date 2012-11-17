#include "gsmmodemmanager.h"
#include "ui_gsmmodemmanager.h"
#include <QMenu>
#include <QMessageBox>
#include "serialportcomboboxdelegate.h"
#include "sendsms.h"
GsmModemManager::GsmModemManager(const QString &dbConnectionString, QWidget *parent) :
    QDialog(parent),
    SqlExtension(dbConnectionString),
    ui(new Ui::GsmModemManager)
{
    ui->setupUi(this);
    sendSMS = new SendSMS(this);
    setWindowTitle(trUtf8("Управление GSM модемами"));
    setWindowFlags(Qt::Dialog
                   | Qt::WindowTitleHint
                   | Qt::WindowCloseButtonHint
                   | Qt::CustomizeWindowHint);
    setWindowModality(Qt::WindowModal);

    model = new ItemModelForGSMModemManagement(dbConnectionString, this);
    ui->treeView->setModel(model);
    ui->treeView->setItemDelegateForColumn(1, new SerialPortComboBoxDelegate(this));
    ui->treeView->resizeColumnToContents(0);
    ui->treeView->resizeColumnToContents(1);
   // connect(ui->pushButtonAdd, SIGNAL(clicked()), this, SLOT(onAddButtonPushed()));
   // connect(ui->pushButtonRemove, SIGNAL(clicked()), this, SLOT(onRemoveButtonPushed()));
   // connect(ui->pushButtonAccept, SIGNAL(clicked()), this, SLOT(accept()));
   // connect(ui->pushButtonReject, SIGNAL(clicked()), this, SLOT(reject()));
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onCustomContextMenuRequested(QPoint)));
    connect(model, SIGNAL(sameModemNameIsPresent(QString)), this, SLOT(onSameModemNameIsPresent(QString)));
    connect(sendSMS, SIGNAL(CMEError(QString,int)), SLOT(onCMEError(QString,int)));
    connect(sendSMS, SIGNAL(CMSError(QString,int)), SLOT(onCMSError(QString,int)));
    connect(sendSMS, SIGNAL(error(QString,QString)), SLOT(onError(QString,QString)));
    connect(sendSMS, SIGNAL(errorWhileSendingSMS(QString,int)), SLOT(onErrorWhileSendingSMS(QString,int)));
    connect(sendSMS, SIGNAL(incomingRing(QString,QString)), SLOT(onIncomingRing(QString,QString)));
    connect(sendSMS, SIGNAL(incomingSMS(QString,QString,QString)), SLOT(onIncomingSMS(QString,QString,QString)));
    connect(sendSMS, SIGNAL(isModemResponse(QString,bool)), SLOT(onIsModemResponse(QString,bool)));
    connect(sendSMS, SIGNAL(operatorName(QString,QString)), SLOT(onOperatorName(QString,QString)));
    connect(sendSMS, SIGNAL(isRegisteredInNetwork(QString,int)), SLOT(onIsRegisteringInNetwork(QString,int)));
    connect(sendSMS, SIGNAL(portIsLost(QString)), SLOT(onPortIsLost(QString)));
    connect(sendSMS, SIGNAL(portIsRestored(QString)), SLOT(onPortIsRestored(QString)));
    connect(sendSMS, SIGNAL(portStateChanged(QString,bool)), SLOT(onPortStateChanged(QString,bool)));
    connect(sendSMS, SIGNAL(signalLevels(QString,int,int)), SLOT(onSignalLevels(QString,int,int)));
    connect(sendSMS, SIGNAL(smsDelivered(QString,int,bool)), SLOT(onSmsDelivered(QString,int,bool)));
    connect(sendSMS, SIGNAL(smsSended(QString,int)), SLOT(onSmsSended(QString,int)));
    connect(sendSMS, SIGNAL(startSendingSMS(QString,int)), SLOT(onStartSendingSMS(QString,int)));
}

GsmModemManager::~GsmModemManager()
{
    delete ui;
}

void GsmModemManager::onAddButtonPushed()
{
#ifdef Q_OS_UNIX
    QString port = "ttySn";
#else
    QString port = "COMn";
#endif
    model->addModem("Имя модема", port, false);
}

void GsmModemManager::onRemoveButtonPushed()
{
    model->deleteModem(ui->treeView->currentIndex());
}

void GsmModemManager::onCustomContextMenuRequested(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);
    connect(menu, SIGNAL(aboutToHide()), menu, SLOT(deleteLater()));
    QModelIndex ind = ui->treeView->indexAt(pos);
    menu->addAction(trUtf8("Добавить модем"), this, SLOT(onAddButtonPushed()));
    if (ind.isValid())
    {
        menu->addAction(trUtf8("Удалить модем"), this, SLOT(onRemoveButtonPushed()));
    }
    menu->exec(QCursor::pos());
}

void GsmModemManager::onDeleteModem(const QModelIndex &index)
{
    model->deleteModem(index);
}

void GsmModemManager::onSameModemNameIsPresent(const QString &modemName)
{
    QMessageBox::information(this, trUtf8("Ошибка"), trUtf8("Модем с именем \"%0\" уже присутствует в системе").arg(modemName));
}

ItemModelForGSMModemManagement::ItemModelForGSMModemManagement(const QString &dbConnectionString, QObject *parent) :
    QStandardItemModel(parent),
    SqlExtension(dbConnectionString)
{
    connect(this, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(onItemChanged(QStandardItem*)));
    refreshModel();


}

void ItemModelForGSMModemManagement::refreshModel()
{
    clear();
    setColumnCount(3);
    setHorizontalHeaderLabels(QStringList()
                              << trUtf8("Активно")
                              << trUtf8("Посл. порт")
                              << trUtf8("Имя модема"));
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.exec("select modem_id, modem_name, modem_port, modem_is_active from gsm_modem");
    int row = 0;
    while(q.next())
    {
        insertRow(row);
        blockSignals(true);
        int id = q.value(0).toInt();
        QStandardItem *item = new QStandardItem(q.value(1).toString());
        item->setData(id, Qt::UserRole + 1);
        item->setData(q.value(1), Qt::UserRole + 2);

        setItem(row, 2, item);

        item = new QStandardItem(q.value(2).toString());
        item->setData(id, Qt::UserRole + 1);
        setItem(row, 1, item);

        item = new QStandardItem();
        item->setCheckState(q.value(3).toBool() ? Qt::Checked : Qt::Unchecked);
        item->setEditable(false);
        item->setCheckable(true);
        item->setData(id, Qt::UserRole + 1);
        setItem(row, 0, item);
        ++row;
        blockSignals(false);
    }
}

void ItemModelForGSMModemManagement::addModem(QString modemName, const QString &portName, const bool &isActive)
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;

    /* проверка на присутствие уже такого имени модема */

    bool first = true;
    for (int i = 0; i < rowCount(); ++i)
    {
        QStandardItem *it = itemFromIndex(index(i, 2));
        if (it->text() == modemName)
        {
            if (first){
                modemName.append(" 1");
            i = -1;
            first = false;
            } else {
                QString num;
                int x = modemName.length() - 1;
                while (modemName.at(x) >= '0' && modemName.at(x) <='9'){
                    num.prepend(modemName.at(x));
                    modemName.remove(x, 1);
                    --x;
                }
                int n = num.toInt();
                ++n;
                num = QString::number(n);
                modemName.append(num);
            }
            continue;
        }
    }

    q.prepare("select id from add_gsm_modem(?, ?, ?)");
    q.addBindValue(modemName);
    q.addBindValue(portName);
    q.addBindValue(isActive);
    q.exec();
    if (!q.next())
        return;
    int id = q.value(0).toInt();
    int row = rowCount();
    insertRow(row);
    blockSignals(true);
    QStandardItem *item = new QStandardItem(modemName);
    item->setData(id, Qt::UserRole + 1);
    item->setData(modemName, Qt::UserRole + 2);
    setItem(row, 2, item);

    item = new QStandardItem(portName);
    item->setData(id, Qt::UserRole + 1);
    setItem(row, 1, item);

    item = new QStandardItem();
    item->setCheckState(isActive ? Qt::Checked : Qt::Unchecked);
    item->setEditable(false);
    item->setCheckable(true);
    item->setData(id, Qt::UserRole + 1);
    setItem(row, 0, item);
    blockSignals(false);
}

void ItemModelForGSMModemManagement::deleteModem(const QModelIndex &index)
{
    if (!index.isValid())
        return;
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    bool ok;
    int id = index.data(Qt::UserRole + 1).toInt(&ok);
    if (!ok)
        return;
    blockSignals(true);
    removeRow(index.row());
    q.prepare("delete from gsm_modem where modem_id = ?");
    q.addBindValue(id);
    q.exec();
    blockSignals(false);
}

void ItemModelForGSMModemManagement::onItemChanged(QStandardItem *item)
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    int id = item->data(Qt::UserRole + 1).toInt();
    switch (item->column())
    {
    case 0: // модем активен/не активен
    {
        q.prepare("update gsm_modem set modem_is_active = ? where modem_id = ?");
        q.addBindValue(item->checkState() == Qt::Checked ? true : false);
        q.addBindValue(id);
        q.exec();
    }
        break;
    case 1: // порт модема
    {
        q.prepare("update gsm_modem set modem_port = ? where modem_id = ?");
        q.addBindValue(item->text());
        q.addBindValue(id);
        q.exec();
    }
        break;
    case 2: // имя модема
    {
        for (int i = 0; i < rowCount(); ++i)
        {
            QStandardItem *it = itemFromIndex(index(i, 2));
            if (it->row() == item->row())
                continue;
            if (it->text() == item->text())
            {
                // тоже самое имя, не принимаем;
                emit sameModemNameIsPresent(item->text());
                blockSignals(true);
                item->setText(item->data(Qt::UserRole + 2).toString());
                blockSignals(false);
                return;
            }
        }
        blockSignals(true);
        item->setData(item->text(), Qt::UserRole + 2);
        blockSignals(false);
        q.prepare("update gsm_modem set modem_name = ? where modem_id = ?");
        q.addBindValue(item->text());
        q.addBindValue(id);
        q.exec();
    }
        break;
    }
}

QVariant ItemModelForGSMModemManagement::data(const QModelIndex &index, int role) const
{
    QVariant value = QStandardItemModel::data(index, role);
    if (role == Qt::BackgroundRole) {  // Цвет фона
        if (index.row() % 2 == 1)
            return QBrush(QColor(Qt::white).lighter(90));
    }
    return value;
}

void GsmModemManager::onSmsDelivered(QString modemName, int idSMS, bool delivered)
{
    qDebug() << QString("onSmsDelivered(%1, %2, %3)").arg(modemName).arg(idSMS).arg(delivered);
}

void GsmModemManager::onSmsSended(QString modemName, int idSMS)
{
    qDebug() << QString("onSmsSended(%1, %2)").arg(modemName).arg(idSMS);
}

void GsmModemManager::onStartSendingSMS(QString modemName, int idSMS)
{
    qDebug() << QString("onStartSendingSMS(%1, %2)").arg(modemName).arg(idSMS);
}

void GsmModemManager::onErrorWhileSendingSMS(QString modemName, int idSMS)
{
    qDebug() << QString("onErrorWhileSendingSMS(%1, %2)").arg(modemName).arg(idSMS);
}

void GsmModemManager::onIncomingRing(QString modemName, QString telNumber)
{
    qDebug() << QString("onIncomingRing(%1, %2)").arg(modemName).arg(telNumber);
}

void GsmModemManager::onIncomingSMS(QString modemName, QString phoneNumber, QString smsTest)
{
     qDebug() << QString("onIncomingSMS(%1, %2, %3)").arg(modemName).arg(phoneNumber).arg(smsTest);
}

void GsmModemManager::onSignalLevels(QString modemName, int rssi, int bor)
{
     qDebug() << QString("onSignalLevels(%1, %2, %3)").arg(modemName).arg(rssi).arg(bor);
}

void GsmModemManager::onPortStateChanged(QString modemName, bool isOpen)
{
     qDebug() << QString("onPortStateChanged(%1, %2)").arg(modemName).arg(isOpen);
}

void GsmModemManager::onIsModemResponse(QString modemName, bool response)
{
     qDebug() << QString("onIsModemResponse(%1, %2)").arg(modemName).arg(response);
}

void GsmModemManager::onPortIsLost(QString modemName)
{
     qDebug() << QString("onPortIsLost(%1)").arg(modemName);
}

void GsmModemManager::onPortIsRestored(QString modemName)
{
    qDebug() << QString("onPortIsRestored(%1)").arg(modemName);
}

void GsmModemManager::onCMEError(QString modemName, int errorNum)
{
    qDebug() << QString("onCMEError(%1, %2)").arg(modemName).arg(errorNum);
}

void GsmModemManager::onCMSError(QString modemName, int errorNum)
{
    qDebug() << QString("onCMSError(%1, %2)").arg(modemName).arg(errorNum);
}

void GsmModemManager::onError(QString modemName, QString lastCommand)
{
    qDebug() << QString("onError(%1, %2)").arg(modemName).arg(lastCommand);
}

void GsmModemManager::onIsRegisteringInNetwork(QString modemName, int status)
{
    qDebug() << QString("onIsRegisteringInNetwork(%1, %2)").arg(modemName).arg(status);
}

void GsmModemManager::onOperatorName(QString modemName, QString operatorN)
{
    qDebug() << QString("onOperatorName(%1, %2)").arg(modemName).arg(operatorN);
}

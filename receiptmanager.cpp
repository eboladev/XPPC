#include "receiptmanager.h"
#include "ui_receiptmanager.h"
#include "setupmanager.h"
#include "branchselectionwidget.h"
#include <QStandardItemModel>

ReceiptManager::ReceiptManager(const QString dbConnectionsString, const int id, QWidget *parent) :
    QDialog(parent),
    SqlExtension(dbConnectionsString),
    currentTicketId(id),
    ui(new Ui::ReceiptManager)
{
    ui->setupUi(this);

    setupConnections();
    adjustSize();

    if (id == -1)
    {
        editMode = false;
        setWindowTitle("Добавление квитанции");
        ui->deviceWidget->setDeviceControlVisible(true);
    }
    else
    {
        editMode = true;
        ui->deviceWidget->setDeviceControlVisible(false);
        QSqlQuery q;
        if (!getSqlQuery(q))
            return;
        q.prepare("select ticket_id from tdc_relation where id = ?");
        q.addBindValue(id);
        q.exec();
        q.next();
        setWindowTitle(trUtf8("Управление квитанцией №").append(QString::number(q.value(0).toInt())));
        fillFields(id);
    }
}

ReceiptManager::~ReceiptManager()
{
    delete ui;
}

void ReceiptManager::clearFields()
{  
    ui->customerWidget->setName("");
    ui->customerWidget->setPhone("");
    ui->deviceWidget->setDeviceName("");
    ui->deviceWidget->setDeviceSerialNumber("");
    ui->deviceWidget->setDeviceProblem("");
    ui->deviceWidget->setDeviceCondition("");  
    setWindowTitle("Добавление квитанции");
}

void ReceiptManager::onAccept()
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;

    Ticket t;
    t.bId = ui->branchSelectionWIdget->getCurrentBranch().toInt();
    t.cId = ui->customerWidget->getCurrentClientId();
    t.cName = ui->customerWidget->getName();
    t.cPhone = ui->customerWidget->getPhone();
    t.dCond = ui->deviceWidget->getDeviceCondition();
    t.dName = ui->deviceWidget->getDeviceName();
    t.dProblem = ui->deviceWidget->getDeviceProblem();
    t.dSN = ui->deviceWidget->getDeviceSerialNumber();
    t.tId = currentTicketId;
    if (editMode)
    {
        q.prepare("select device.id from device join tdc_relation on (tdc_relation.device_id = device.id) where tdc_relation.id = ?");
        q.addBindValue(currentTicketId);
        q.exec();
        q.next();
        t.dId = q.value(0).toInt();
    }

    if (t.cId == -1)
    {
        q.prepare("insert into client(name,phone) values(?,?) returning id");
        q.addBindValue(t.cName);
        q.addBindValue(t.cPhone);
        if (!q.exec())
            qDebug() << q.lastError() << q.lastQuery();
        q.next();
        t.cId = q.value(0).toInt();
     }
    else
    {
        q.prepare("update client set name = ?, phone = ? where id = ?");
        q.addBindValue(t.cName);
        q.addBindValue(t.cPhone);
        q.addBindValue(t.cId);
        if (!q.exec())
            qDebug() << q.lastError() << q.lastQuery();
    }

    if (editMode)
    {
        q.prepare("update device set name = ?, problem = ?, serial = ?, condition = ?, branch_id = ? where id = ?");
        q.addBindValue(t.dName);
        q.addBindValue(t.dProblem);
        q.addBindValue(t.dSN);
        q.addBindValue(t.dCond);
        q.addBindValue(t.bId);
        q.addBindValue(t.dId);
        if (!q.exec())
            qDebug() << q.lastError() << q.lastQuery();
    }
    else
    {
        if (ui->deviceWidget->deviceModel->rowCount() > 0)
        {
            QStandardItemModel* dm = ui->deviceWidget->deviceModel;
            for (int i = 0 ; i < dm->rowCount(); ++i)
            {
                q.prepare("insert into device(name,problem,serial,condition,branch_id) values(?,?,?,?,?) returning id");
                q.addBindValue(dm->item(i,0)->text());
                q.addBindValue(dm->item(i,0)->data());
                q.addBindValue(dm->item(i,2)->text());
                q.addBindValue(dm->item(i,1)->text());
                q.addBindValue(t.bId);
                if (!q.exec())
                    qDebug() << q.lastError() << q.lastQuery();
                q.next();
                deviceIdList.append(q.value(0).toInt());
            }
        }
        else
        {
            q.prepare("insert into device(name,problem,serial,condition,branch_id) values(?,?,?,?) returning id");
            q.addBindValue(t.dName);
            q.addBindValue(t.dProblem);
            q.addBindValue(t.dSN);
            q.addBindValue(t.dCond);
            q.addBindValue(t.bId);
            if (!q.exec())
                qDebug() << q.lastError() << q.lastQuery();
            q.next();
            t.dId = q.value(0).toInt();
        }
    }
    if (editMode)
    {
        q.prepare("update device set branch_id = ? where id = ?");
        q.addBindValue(t.bId);
        q.addBindValue(t.dId);
        if (!q.exec())
            qDebug() << q.lastError() << q.lastQuery();
    }
    else
    {
        q.prepare("insert into ticket(status) values(0) returning id;");
        if (!q.exec())
            qDebug() << q.lastError() << q.lastQuery();
        q.next();
        t.tId = q.value(0).toInt();
    }

    if (!editMode)
    {
        if (t.dId > 0)
        {
            q.prepare("insert into tdc_relation(ticket_id,client_id,device_id) values(?,?,?)");
            q.addBindValue(t.tId);
            q.addBindValue(t.cId);
            q.addBindValue(t.dId);
            if (!q.exec())
                qDebug() << q.lastError() << q.lastQuery();
        }
        else
        {
            while (!deviceIdList.isEmpty())
            {
                q.prepare("insert into tdc_relation(ticket_id,client_id,device_id) values(?,?,?)");
                q.addBindValue(t.tId);
                q.addBindValue(t.cId);
                q.addBindValue(deviceIdList.takeFirst());
                if (!q.exec())
                    qDebug() << q.lastError() << q.lastQuery();
            }
        }
    }

    accept();
}

void ReceiptManager::fillFields(int id)
{    
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.prepare("select "
              "client.name, client.phone, device.name, device.serial, "
              "device.problem, device.condition, branch.id, client.id "
              "from tdc_relation "
              "join ticket on(tdc_relation.ticket_id = ticket.id) "
              "join client on(tdc_relation.client_id = client.id) "
              "join device on(tdc_relation.device_id = device.id) "
              "join branch on (device.branch_id = branch.id) "
              "where tdc_relation.id = ?");
    q.addBindValue(id);
    if (!q.exec())
        qDebug() << q.lastError() << q.lastQuery();
    if (!q.next())
        return;
    ui->customerWidget->setName(q.value(0).toString());
    ui->customerWidget->setPhone(q.value(1).toString());
    ui->customerWidget->setCurrentClientId(q.value(7).toInt());
    ui->deviceWidget->setDeviceName(q.value(2).toString());
    ui->deviceWidget->setDeviceSerialNumber(q.value(3).toString());
    ui->deviceWidget->setDeviceProblem(q.value(4).toString());
    ui->deviceWidget->setDeviceCondition(q.value(5).toString());
    ui->branchSelectionWIdget->setBranch(q.value(6).toString());
}

void ReceiptManager::setupConnections()
{         
    connect(ui->pushButtonCancel,SIGNAL(clicked()), SLOT(reject()));
    connect(ui->pushButtonOk, SIGNAL(clicked()), SLOT(onAccept()));
}

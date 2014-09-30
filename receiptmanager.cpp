#include "receiptmanager.h"
#include "ui_receiptmanager.h"
#include "setupmanager.h"
#include "branchselectionwidget.h"
#include "globals.h"

#include <QMessageBox>
#include <QStandardItemModel>

ReceiptManager::ReceiptManager(const QString dbConnectionsString, const int &tdcr_id, QWidget *parent) :
    QDialog(parent),
    SqlExtension(dbConnectionsString),
    currentTicketId(tdcr_id),
    ui(new Ui::ReceiptManager)
{
    ui->setupUi(this);

    setupConnections();
    adjustSize();

    if (tdcr_id == -1)
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
        q.prepare("select ticket_id from ticket where id = ?");
        q.addBindValue(tdcr_id);
        q.exec();
        q.next();
        setWindowTitle(trUtf8("Управление квитанцией №").append(QString::number(q.value(0).toInt())));
        fillFields(tdcr_id);
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
    if (ui->deviceWidget->getDeviceCondition().trimmed().isEmpty()) {
        QMessageBox::critical(this,trUtf8("Ошибка валидации"),trUtf8("Поле \"Дефекты\" должно быть заполнено."));
        return;
    }
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;

    Ticket t;
    t.dId = -1;
    t.bId = ui->branchSelectionWIdget->getCurrentBranch().toInt();
    t.cId = ui->customerWidget->getCurrentClientId();
    t.cName = ui->customerWidget->getName();
    t.cPhone = ui->customerWidget->getPhone();
    t.dCond = ui->deviceWidget->getDeviceCondition();
    t.dName = ui->deviceWidget->getDeviceName();
    t.dProblem = ui->deviceWidget->getDeviceProblem();
    t.dSN = ui->deviceWidget->getDeviceSerialNumber();
    t.tId = currentTicketId;
    t.ctId = -1;

    if (editMode)
    {
        qDebug() << currentTicketId;
        q.prepare("select device.id from device join ticket on (ticket.device_id = device.id) where ticket.id = ?");
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
        q.prepare("insert into client_ticket(client_id) values(?) returning id");
        q.addBindValue(t.cId);
        if (!q.exec())
            qDebug() << q.lastError() << q.lastQuery();
        q.next();
        t.ctId = q.value(0).toInt(); //№ квитанции
     }
    else
    {//такой клиент уже есть в базе
        if (editMode)
        {
            q.prepare("update client set name = ?, phone = ? where id = ?");
            q.addBindValue(t.cName);
            q.addBindValue(t.cPhone);
            q.addBindValue(t.cId);
            if (!q.exec())
                qDebug() << q.lastError() << q.lastQuery();
        }
        else
        {
            q.prepare("insert into client_ticket(client_id) values(?) returning id");
            q.addBindValue(t.cId);
            if (!q.exec())
                qDebug() << q.lastError() << q.lastQuery();
            q.next();
            t.ctId = q.value(0).toInt(); //№ квитанции
        }
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
            q.prepare("insert into device(name,problem,serial,condition,branch_id) values(?,?,?,?,?) returning id");
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
    /*else
    {
        q.prepare("insert into ticket(status) values(?) returning id;");
        q.addBindValue(InWork);
        if (!q.exec())
            qDebug() << q.lastError() << q.lastQuery();
        q.next();
        t.tId = q.value(0).toInt();
    }*/

    if (!editMode)
    {
        if (t.dId > 0)
        {
            q.prepare("insert into ticket(device_id,ticket_id) VALUES(?,?)");
            q.addBindValue(t.dId);
            q.addBindValue(t.ctId);
            if (!q.exec())
                qDebug() << q.lastError() << q.lastQuery();
        }
        else
        {
            while (!deviceIdList.isEmpty())
            {
                q.prepare("insert into ticket(device_id,ticket_id) VALUES(?,?)");
                q.addBindValue(deviceIdList.takeFirst());
                q.addBindValue(t.ctId);
                if (!q.exec())
                    qDebug() << q.lastError() << q.lastQuery();
            }
        }
       /* q.exec("select max(ticket_id) from ticket");
        q.next();
        t.tId = q.value(0).toInt() + 1;
        if (t.dId > 0)
        {
            q.prepare("insert into ticket(ticket_id,client_id,device_id) values(?,?,?)");
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
                q.prepare("insert into ticket(ticket_id,client_id,device_id) values(?,?,?)");
                q.addBindValue(t.tId);
                q.addBindValue(t.cId);
                q.addBindValue(deviceIdList.takeFirst());
                if (!q.exec())
                    qDebug() << q.lastError() << q.lastQuery();
            }
        }*/
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
              "from client_ticket "
              "join client on(client_ticket.client_id = client.id) "
              "INNER JOIN ticket ON (ticket.ticket_id = client_ticket.id) "
              "join device on(ticket.device_id = device.id) "
              "join branch on (device.branch_id = branch.id) "
              "where ticket.id = ?");
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

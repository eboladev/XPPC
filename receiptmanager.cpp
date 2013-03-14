#include "receiptmanager.h"
#include "ui_receiptmanager.h"
#include "setupmanager.h"
#include "branchselectionwidget.h"

ReceiptManager::ReceiptManager(const QString dbConnectionsString, const int id, QWidget *parent) :
    QDialog(parent),
    SqlExtension(dbConnectionsString),
    ui(new Ui::ReceiptManager)
{
    ui->setupUi(this);                 

    setupConnections();
    initWidgets();

    if (id == -1)
    {
        setWindowTitle("Добавление квитанции");
    }
    else
    {
        setWindowTitle(trUtf8("Управление квитанцией №").append(QString::number(id)));
        ui->pushButtonUpdate->setEnabled(true);
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
    ui->pushButtonUpdate->setEnabled(false);
}

void ReceiptManager::fillFields(int id)
{    
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.prepare("select "
              "client.name, client.phone, device.name, device.serial, "
              "device.problem, device.condition, branch.id "
              "from tdc_relation "
              "join ticket on(tdc_relation.ticket_id = ticket.id) "
              "join client on(tdc_relation.client_id = client.id) "
              "join device on(tdc_relation.device_id = device.id) "
              "join branch on (ticket.branch = branch.id) "
              "where tdc_relation.id = ?");
    q.addBindValue(id);
    q.exec();
    if (!q.next())
        return;
    ui->customerWidget->setName(q.value(0).toString());
    ui->customerWidget->setPhone(q.value(1).toString());
    ui->deviceWidget->setDeviceName(q.value(2).toString());
    ui->deviceWidget->setDeviceSerialNumber(q.value(3).toString());
    ui->deviceWidget->setDeviceProblem(q.value(4).toString());
    ui->deviceWidget->setDeviceCondition(q.value(5).toString());
    ui->branchSelectionWIdget->setBranch(q.value(6).toString());
}

void ReceiptManager::onAddReceiptClicked()
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;

}

void ReceiptManager::setupConnections()
{      
    connect(ui->pushButtonAddReceipt, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->pushButtonClose,SIGNAL(clicked()),this, SLOT(reject()));
}

void ReceiptManager::initWidgets()
{        
    ui->groupBoxDeviceList->setVisible(false);
    ui->groupBoxDeviceList_2->setVisible(false);
    adjustSize();    
}

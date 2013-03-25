#include "reportssettings.h"
#include "ui_reportssettings.h"
#include "reportselectionwidget.h"
#include "globals.h"

#include <QDebug>

ReportsSettings::ReportsSettings(const QString &dbConenctionString, QWidget *parent) :
    QDialog(parent),
    SqlExtension(dbConenctionString),
    ui(new Ui::ReportsSettings)
{
    ui->setupUi(this);
    connect(ui->pushButtonOk, SIGNAL(clicked()), SLOT(onAccept()));
    connect(ui->pushButtonCancel, SIGNAL(clicked()), SLOT(reject()));
    fillReportSelectWidgets();
    adjustSize();
}

ReportsSettings::~ReportsSettings()
{
    delete ui;
}

void ReportsSettings::onAccept()
{
    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    for (int i = TicketReport; i != CashCheckReport + 1; ++i)
    {
        q.prepare("select report_path from reports where branch_id = ? and report_type = ?");
        q.addBindValue(ui->branchSelectionWidget->getCurrentBranch());
        q.addBindValue(i);
        if (!q.exec())
            qDebug() << q.lastError() << q.lastQuery();
        if (!q.next())
        {
            QSqlQuery q2;
            if (!getSqlQuery(q2))
                return;
            q2.prepare("insert into reports(report_path,report_type,branch_id) VALUES(?,?,?)");
            q2.addBindValue(reportsMap[i]->getReportPath());
            q2.addBindValue(i);
            q2.addBindValue(ui->branchSelectionWidget->getCurrentBranch());
            if (!q2.exec())
                qDebug() << q2.lastError() << q2.lastQuery();
        }
        else
        {
            QSqlQuery q2;
            if (!getSqlQuery(q2))
                return;
            q2.prepare("update reports set report_path = ? where report_type = ? and branch_id = ?");
            q2.addBindValue(reportsMap[i]->getReportPath());
            q2.addBindValue(i);
            q2.addBindValue(ui->branchSelectionWidget->getCurrentBranch());
            if (!q2.exec())
                qDebug() << q2.lastError() << q2.lastQuery();
        }
    }
    accept();
}

void ReportsSettings::fillReportSelectWidgets()
{
    ReportSelectionWidget* rswTicketRep = new ReportSelectionWidget(ui->widgetTicketRep);
    rswTicketRep->setGroupBoxTitle(trUtf8("Квитанции"));
    reportsMap[TicketReport] = rswTicketRep;
    ReportSelectionWidget* rswJobListRep = new ReportSelectionWidget(ui->widgetJobListRep);
    rswJobListRep->setGroupBoxTitle(trUtf8("Акт выполненных работ"));
    reportsMap[JobListReport] = rswJobListRep;
    ReportSelectionWidget* rswCashReceiptRep = new ReportSelectionWidget(ui->widgetCashReceiptRep);
    rswCashReceiptRep->setGroupBoxTitle(trUtf8("Приходный кассовый ордер"));
    reportsMap[CashReceiptReport] = rswCashReceiptRep;
    ReportSelectionWidget* rswPriceTagRep = new ReportSelectionWidget(ui->widgetPriceTagRep);
    rswPriceTagRep->setGroupBoxTitle(trUtf8("Ценник"));
    reportsMap[PricetTagReport] = rswPriceTagRep;
    ReportSelectionWidget* rswCashCheckRep = new ReportSelectionWidget(ui->widgetCashCheckRep);
    rswCashCheckRep->setGroupBoxTitle(trUtf8("Товарный чек"));
    reportsMap[CashCheckReport] = rswCashCheckRep;

    QSqlQuery q;
    if (!getSqlQuery(q))
        return;
    q.prepare("select report_type, report_path from reports where branch_id = ?");
    q.addBindValue(ui->branchSelectionWidget->getCurrentBranch());
    if (!q.exec())
        qDebug() << q.lastError() << q.lastQuery();
    while (q.next())
    {
        switch(q.value(0).toInt())
        {
        case TicketReport:
        {
            rswTicketRep->setReportPath(q.value(1).toString());
        }
            break;
        case JobListReport:
        {
            rswJobListRep->setReportPath(q.value(1).toString());
        }
            break;
        case CashReceiptReport:
        {
            rswCashReceiptRep->setReportPath(q.value(1).toString());
        }
            break;
        case PricetTagReport:
        {
            rswPriceTagRep->setReportPath(q.value(1).toString());
        }
            break;
        case CashCheckReport:
        {
            rswCashCheckRep->setReportPath(q.value(1).toString());
        }
            break;
        }
    }
}

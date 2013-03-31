#include "reportssettings.h"
#include "ui_reportssettings.h"
#include "reportselectionwidget.h"
#include "globals.h"

#include <QSettings>
#include <QDebug>

ReportsSettings::ReportsSettings(QWidget *parent) :
    QDialog(parent),    
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
    QSettings s;
    for (int i = TicketReport; i != CashCheckReport + 1; ++i)
        s.setValue(QString("reports/%0").arg(i),reportsMap[i]->getReportPath());
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

    QSettings s;
    rswTicketRep->setReportPath(s.value("reports/0").toString());
    rswJobListRep->setReportPath(s.value("reports/1").toString());
    rswCashReceiptRep->setReportPath(s.value("reports/2").toString());
    rswPriceTagRep->setReportPath(s.value("reports/3").toString());
    rswCashCheckRep->setReportPath(s.value("reports/4").toString());
}

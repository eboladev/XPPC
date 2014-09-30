#include "reportssettings.h"
#include "ui_reportssettings.h"
#include "reportselectionwidget.h"
#include "globals.h"

#include <QSettings>
#include <QDebug>
#include <QFileDialog>

ReportsSettings::ReportsSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReportsSettings)
{
    ui->setupUi(this);
    connect(ui->pushButtonOk, SIGNAL(clicked()), SLOT(onAccept()));
    connect(ui->pushButtonCancel, SIGNAL(clicked()), SLOT(reject()));
    connect(ui->ticketSelect, SIGNAL(clicked()), this, SLOT(onTicketSelect()));
    connect(ui->casjSelect, SIGNAL(clicked()), this, SLOT(onCashSelect()));
    connect(ui->jobSelect, SIGNAL(clicked()), this, SLOT(onJobSelect()));
    ui->groupBoxBranch->setVisible(false);
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
    s.setValue(QString("reports/0"),ui->lineEditReportPath_Ticket->text());
    s.setValue(QString("reports/1"),ui->lineEditReportPath_Job->text());
    s.setValue(QString("reports/2"),ui->lineEditReportPath_Cash->text());
    accept();
}

void ReportsSettings::onTicketSelect()
{
    QString res;
    executeFileDialog(res);
    ui->lineEditReportPath_Ticket->setText(res);
}

void ReportsSettings::onJobSelect()
{
    QString res;
    executeFileDialog(res);
    ui->lineEditReportPath_Job->setText(res);
}

void ReportsSettings::onCashSelect()
{
    QString res;
    executeFileDialog(res);
    ui->lineEditReportPath_Cash->setText(res);
}

void ReportsSettings::fillReportSelectWidgets()
{
    ui->TicketRepInfo->setPlainText(trUtf8("Формат: %название_закладки% - комментарий\n"
                                           "Для дублирующих полей используется цифровой префикс, например client_name2\n"
                                           "client_name - имя клиента\n"
                                           "client_phone - телефон\n"
                                           "ticket_number - номер квитанции\n"
                                           "current_date - дата заполнения квитанции в формате ДД-ММ-ГГГГ\n"
                                           "Таблица информации по устройству, название - deviceTable. \n"
                                           "столбцы: №, название, серийный номер, дефекты, неисправность\n"
                                           "в случае если таблица отсутсвует используются следующие закладки: \n"
                                           "device_condition - дефект\n"
                                           "device_name - название\n"
                                           "device_problem - неисправность\n"
                                           "device_serial - серийный номер"));

    QSettings s;
    ui->lineEditReportPath_Ticket->setText(s.value("reports/0").toString());
    ui->lineEditReportPath_Job->setText(s.value("reports/1").toString());
    ui->lineEditReportPath_Cash->setText(s.value("reports/2").toString());
}

bool ReportsSettings::executeFileDialog(QString &result)
{
    QFileDialog fd;
    fd.setFileMode(QFileDialog::ExistingFile);
    fd.setFilter(tr("Шаблоны документов (*.dot *.dotx *.ott)"));
    if (!fd.exec()) return false;
    result = fd.selectedFiles().first();
    return true;
}

#include "guaranteeonticketreasonwidget.h"
#include "ui_guaranteeonticketreasonwidget.h"

#include <QStandardItemModel>
#include <QDebug>
#include <QStandardItem>

GuaranteeOnTicketReasonWidget::GuaranteeOnTicketReasonWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GuaranteeOnTicketReasonWidget)
{
    ui->setupUi(this);
    model = new QStandardItemModel(this);
    ui->tableViewTicketList->setModel(model);
    model->setHorizontalHeaderLabels(QStringList() << trUtf8("№") << trUtf8("Название") << trUtf8("S\N"));
    connect(ui->tableViewTicketList->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(onCurrentTicketChanged(QModelIndex,QModelIndex)));
    connect(ui->pushButtonCancel, SIGNAL(clicked()), SLOT(reject()));
    connect(ui->pushButtonOk, SIGNAL(clicked()), SLOT(onAccept()));
}

GuaranteeOnTicketReasonWidget::~GuaranteeOnTicketReasonWidget()
{
    delete ui;
}

void GuaranteeOnTicketReasonWidget::addDevice(const QVariant &id,
                                              const int &ticket_id,
                                              const QString &name,
                                              const QString &serial)
{
    QStandardItem* itemid = new QStandardItem(QString::number(ticket_id));
    itemid->setData(id);
    QStandardItem* nameitem = new QStandardItem(name);
    QStandardItem* serialitem = new QStandardItem(serial);
    ticketGuaranteeText.insert(id.toString(),"");
    currentId = id;
    ui->tableViewTicketList->resizeColumnsToContents();
    model->appendRow(QList<QStandardItem*>() << itemid << nameitem << serialitem);
}

void GuaranteeOnTicketReasonWidget::onCurrentTicketChanged(QModelIndex current, QModelIndex previous)
{
    if(!current.isValid())
        return;
    currentId = model->item(current.row(),0)->data();
    if(!current.isValid() || !previous.isValid())
        return;
    model->item(previous.row(),0)->setData(ui->plainTextEditProblem->toPlainText(),Qt::UserRole + 2);
    ui->plainTextEditProblem->setPlainText(model->item(current.row(),0)->data(Qt::UserRole + 2).toString());
    ticketGuaranteeText[model->item(previous.row(),0)->data().toString()] = model->item(previous.row(),0)->data(Qt::UserRole + 2).toString();
}

void GuaranteeOnTicketReasonWidget::onAccept()
{
    qDebug() << currentId;
    ticketGuaranteeText[currentId.toString()] = ui->plainTextEditProblem->toPlainText();
    accept();
}

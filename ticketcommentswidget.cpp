#include "ticketcommentswidget.h"
#include "ui_ticketcommentswidget.h"

TicketCommentsWidget::TicketCommentsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TicketCommentsWidget)
{
    ui->setupUi(this);
}

TicketCommentsWidget::~TicketCommentsWidget()
{
    delete ui;
}

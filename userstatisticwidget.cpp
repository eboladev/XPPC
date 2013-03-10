#include "userstatisticwidget.h"
#include "ui_userstatisticwidget.h"

UserStatisticWidget::UserStatisticWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserStatisticWidget)
{
    ui->setupUi(this);
}

UserStatisticWidget::~UserStatisticWidget()
{
    delete ui;
}

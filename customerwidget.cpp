#include "customerwidget.h"
#include "ui_customerwidget.h"
#include <QDebug>
CustomerWidget::CustomerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomerWidget)
{
    ui->setupUi(this);
}

CustomerWidget::~CustomerWidget()
{
    delete ui;
}

void CustomerWidget::setName(QString name)
{    
    qDebug() << "setname";
    qDebug() << name;
    ui->lineEditName->setText(name);
}

void CustomerWidget::setPhone(QString phone)
{
    ui->lineEditPhone->setText(phone.trimmed());
}

QString CustomerWidget::getName() const
{
    return ui->lineEditName->text().trimmed();
}

QString CustomerWidget::getPhone() const
{
    return ui->lineEditPhone->text().trimmed();
}

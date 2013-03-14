#include "customerwidget.h"

#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>

CustomerWidget::CustomerWidget(QWidget *parent) :
    QWidget(parent)
{
    nameLabel = new QLabel(trUtf8("ФИО:"), this);
    phoneLabel = new QLabel(trUtf8("Телефон:"), this);
    nameEdit = new QLineEdit(this);
    phoneEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText(trUtf8("ФИО:"));
    phoneEdit->setPlaceholderText(trUtf8("Телефон:"));
    QVBoxLayout* vl = new QVBoxLayout();
    QHBoxLayout* horNameLayout = new QHBoxLayout();
    horNameLayout->addWidget(nameLabel);
    horNameLayout->addWidget(nameEdit);
    QHBoxLayout* horPhoneLayout= new QHBoxLayout();
    horPhoneLayout->addWidget(phoneLabel);
    horPhoneLayout->addWidget(phoneEdit);
    vl->addLayout(horNameLayout);
    vl->addLayout(horPhoneLayout);
    setLayout(vl);
}

CustomerWidget::~CustomerWidget()
{
}

void CustomerWidget::setName(const QString &name)
{    
    nameEdit->setText(name);
}

void CustomerWidget::setPhone(const QString &phone)
{
    phoneEdit->setText(phone);
}

QString CustomerWidget::getName() const
{
    return nameEdit->text();
}

QString CustomerWidget::getPhone() const
{
    return phoneEdit->text();
}

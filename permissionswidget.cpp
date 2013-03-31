#include "permissionswidget.h"
#include "ui_permissionswidget.h"

PermissionsWidget::PermissionsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PermissionsWidget)
{
    ui->setupUi(this);
}

PermissionsWidget::~PermissionsWidget()
{
    delete ui;
}

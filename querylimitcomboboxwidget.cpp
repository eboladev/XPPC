#include "querylimitcomboboxwidget.h"
#include "ui_querylimitcomboboxwidget.h"

QueryLimitComboBoxWidget::QueryLimitComboBoxWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QueryLimitComboBoxWidget)
{
    ui->setupUi(this);    
    connect(ui->comboBoxQueryLimit, SIGNAL(currentIndexChanged(int)), SIGNAL(limitChanged(int)));
}

QueryLimitComboBoxWidget::~QueryLimitComboBoxWidget()
{
    delete ui;
}

int QueryLimitComboBoxWidget::getLimit()
{
    return ui->comboBoxQueryLimit->currentText().toInt();
}

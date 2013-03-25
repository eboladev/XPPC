#include "reportselectionwidget.h"
#include "ui_reportselectionwidget.h"

#include <QFileDialog>
#include <QDebug>
ReportSelectionWidget::ReportSelectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReportSelectionWidget)
{
    ui->setupUi(this);
    //ui->setupUi(this);
    connect(ui->pushButtonSelect, SIGNAL(clicked()), this, SLOT(onSelectReportClicked()));
    //setGroupBoxTitle(parent->windowTitle());
}

ReportSelectionWidget::~ReportSelectionWidget()
{
    delete ui;
}

void ReportSelectionWidget::setReportPath(const QString &path)
{
    ui->lineEditReportPath->setText(path);
}

void ReportSelectionWidget::setGroupBoxTitle(const QString &text)
{
    ui->groupBox->setTitle(text);
}


QString ReportSelectionWidget::getReportPath() const
{
    return ui->lineEditReportPath->text().trimmed();
}

void ReportSelectionWidget::onSelectReportClicked()
{
    QFileDialog fd;
    fd.setFileMode(QFileDialog::ExistingFile);
    fd.setFilter(tr("Шаблоны документов (*.dot *.dotx *.ott)"));
    if (fd.exec())
        ui->lineEditReportPath->setText(fd.selectedFiles().first());
}

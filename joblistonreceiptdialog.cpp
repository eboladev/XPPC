#include "joblistonreceiptdialog.h"
#include "ui_joblistonreceiptdialog.h"
#include "setupmanager.h"
#include "usersandpermissionsmanager.h"
#include "globals.h"
#include "employeewidget.h"
#include "jobitemmodel.h"
#include "joblistitemmodel.h"

#include <QSortFilterProxyModel>
#include <QCompleter>
#include <QTreeView>

JobListOnReceiptDialog::JobListOnReceiptDialog(const QString dbConnectionsString, const int &id, const int &ticket_id = 0, QWidget *parent) :
    QDialog(parent),
    SqlExtension(dbConnectionsString),
    m_id(id),
    ui(new Ui::JobListOnReceiptDialog)
{
    ui->setupUi(this);    
    jobsModel = new JobItemModel(dbConnectionsString, this);
    ui->tableViewJobs->setModel(jobsModel);
    setWindowTitle(trUtf8("Управление списком работ по квитанции №").append(QString::number(ticket_id)));
    connect(ui->pushButtonOK, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->pushButtonCancel,SIGNAL(clicked()),this, SLOT(reject()));
    connect(ui->pushButtonAddJob, SIGNAL(clicked()), this, SLOT(onPushButtonAddJobClicked()));
    connect(ui->pushButtonDeleteJob, SIGNAL(clicked()), this, SLOT(onPushButtonDeleteJobClicked()));
    connect(ui->pushButtonUpdateJob, SIGNAL(clicked()), this, SLOT(onUpdateClicked()));
    connect(ui->pushButtonClearField, SIGNAL(clicked()), this, SLOT(clearField()));
    connect(ui->tableViewJobs->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(onCurrentSelectionChanged(QModelIndex,QModelIndex)));

    employeeWidget = new EmployeeWidget(dbConnectionsString, ui->employeeWidget);
    employeeWidget->setEmployeeCurrentId();
    ui->employeeWidget->setMinimumHeight(employeeWidget->geometry().height());
    ui->employeeWidget->setMinimumWidth(employeeWidget->geometry().width());
    adjustSize();
    employeeWidget->setEnabled(accessManager->isCanEditJobList());

    //TODO: create completer_with_treeview class
    jobListModel = new JobListItemModel(dbConnectionsString,this);
    jobListProxy = new QSortFilterProxyModel(this);
    jobListProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    jobListProxy->setSourceModel(jobListModel);

    connect(ui->lineEditJobName, SIGNAL(textChanged(QString)), jobListProxy, SLOT(setFilterFixedString(QString)));

    QTreeView* tw = new QTreeView(this);
    tw->setModel(jobListProxy);
    tw->setHeaderHidden(true);
    tw->setIndentation(-1);
    tw->resizeColumnToContents(0);
    QHeaderView* hw = tw->header();
    hw->setStretchLastSection(false);
    hw->setCascadingSectionResizes(true);
    hw->setDefaultSectionSize(150);
    tw->setHeader(hw);

    QCompleter *completer = new QCompleter(jobListProxy, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setPopup(tw);
    completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    ui->lineEditJobName->setCompleter(completer);    
    connect(completer, SIGNAL(activated(QModelIndex)), SLOT(onCompleteJobData(QModelIndex)));
    ui->lineEditJobName->setProperty("jobId",-1);
    getJobs(id);
}

JobListOnReceiptDialog::~JobListOnReceiptDialog()
{
    delete ui;
}

void JobListOnReceiptDialog::getJobs(const QVariant &id)
{
    jobsModel->getJobs(id);
    ui->tableViewJobs->resizeColumnsToContents();
}

bool JobListOnReceiptDialog::checkPermissions(const QVariant &selectedEmployeeId) const
{
    if (!accessManager->isCanEditJobList())
        return accessManager->getCurrentUserId() == selectedEmployeeId;
}

QCompleter *JobListOnReceiptDialog::getCompleter()
{
    return ui->lineEditJobName->completer();
}

void JobListOnReceiptDialog::clearField()
{
    ui->lineEditJobName->clear();
    ui->spinBoxPrice->setValue(1);
    ui->spinBoxQuantity->setValue(1);    
}

void JobListOnReceiptDialog::onPushButtonAddJobClicked()
{
    if (ui->lineEditJobName->text().isEmpty())
        return;

    jobsModel->addJob(m_id,
                      employeeWidget->getCurrentEmployeeId(),
                      ui->lineEditJobName->text().trimmed(),
                      ui->spinBoxQuantity->value(),
                      ui->spinBoxPrice->value(),
                      ui->lineEditGuarantee->text());
    if (!jobListModel->isJobExist(ui->lineEditJobName->text().trimmed()))
        jobListModel->addJob(ui->lineEditJobName->text().trimmed(),
                             ui->spinBoxPrice->value(),
                             ui->groupBoxGuarantee->isChecked(),
                             ui->lineEditGuarantee->text().trimmed());
    getJobs(m_id);
}

void JobListOnReceiptDialog::onPushButtonDeleteJobClicked()
{
    if (!ui->tableViewJobs->currentIndex().isValid())
        return;
    jobsModel->deleteJob(ui->tableViewJobs->currentIndex());
    getJobs(m_id);
}

void JobListOnReceiptDialog::onUpdateClicked()
{
    if (!ui->tableViewJobs->currentIndex().isValid())
        return;
    jobsModel->updateJob(ui->tableViewJobs->currentIndex(),
                         employeeWidget->getCurrentEmployeeId(),
                         ui->lineEditJobName->text().trimmed(),
                         ui->spinBoxQuantity->value(),
                         ui->spinBoxPrice->value());
    getJobs(m_id);
}

void JobListOnReceiptDialog::onCurrentSelectionChanged(QModelIndex current, QModelIndex previous)
{
    Q_UNUSED(previous);
    if (!current.isValid())
        return;

    Job currentJob;
    currentJob.id = jobsModel->item(current.row(),0)->data();
    jobsModel->getCurrentJobName(currentJob);
    ui->lineEditJobName->setText(currentJob.name);
    ui->spinBoxPrice->setValue(currentJob.price);
    ui->spinBoxQuantity->setValue(currentJob.quantity);
    ui->pushButtonDeleteJob->setEnabled(checkPermissions(currentJob.employeeId));
    ui->pushButtonUpdateJob->setEnabled(checkPermissions(currentJob.employeeId));
}

void JobListOnReceiptDialog::onCompleteJobData(QModelIndex index)
{    
    ui->groupBoxGuarantee->setChecked(getCompleter()->
                                      completionModel()->
                                      data(getCompleter()->
                                           completionModel()->
                                           index(index.row(),JobListModelHeader::Name),JobListModelData::HasGuarantee).toBool());
    if (ui->groupBoxGuarantee->isChecked())
        ui->lineEditGuarantee->setText(getCompleter()->
                                       completionModel()->
                                       data(getCompleter()->
                                            completionModel()->
                                            index(index.row(),JobListModelHeader::Name),JobListModelData::GuaranteePeriod).toString());    
    ui->spinBoxPrice->setValue(getCompleter()->
                               completionModel()->
                               data(getCompleter()->
                                    completionModel()->
                                    index(index.row(),JobListModelHeader::Price),JobListModelData::Text).toInt());
    ui->lineEditJobName->setProperty("jobId",getCompleter()->
                                     completionModel()->
                                     data(getCompleter()->
                                          completionModel()->
                                          index(index.row(),JobListModelHeader::Name),JobListModelData::ID));
}

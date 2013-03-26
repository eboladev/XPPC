#include "joblistonreceiptdialog.h"
#include "ui_joblistonreceiptdialog.h"
#include "setupmanager.h"
#include "globals.h"
#include "employeewidget.h"
#include "jobitemmodel.h"
#include "joblistitemmodel.h"

#include <QSortFilterProxyModel>
#include <QCompleter>
#include <QTreeView>

JobListOnReceiptDialog::JobListOnReceiptDialog(const QString dbConnectionsString, const int &id, QWidget *parent) :
    QDialog(parent),
    SqlExtension(dbConnectionsString),
    m_id(id),
    ui(new Ui::JobListOnReceiptDialog)
{
    ui->setupUi(this);    
    jobsModel = new JobItemModel(dbConnectionsString, this);
    ui->tableViewJobs->setModel(jobsModel);
    setWindowTitle(trUtf8("Управление списком работ по квитанции №").append(QString::number(id)));
    connect(ui->pushButtonOK, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->pushButtonCancel,SIGNAL(clicked()),this, SLOT(reject()));
    connect(ui->pushButtonAddJob, SIGNAL(clicked()), this, SLOT(onPushButtonAddJobClicked()));
    connect(ui->pushButtonDeleteJob, SIGNAL(clicked()), this, SLOT(onPushButtonDeleteJobClicked()));
    connect(ui->pushButtonUpdateJob, SIGNAL(clicked()), this, SLOT(onUpdateClicked()));
    connect(ui->pushButtonClearField, SIGNAL(clicked()), this, SLOT(clearField()));
    connect(ui->tableViewJobs->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(onCurrentSelectionChanged(QModelIndex,QModelIndex)));

    connect(ui->lineEditJobName, SIGNAL(textChanged(QString)), this, SLOT(onJobStateHasChanged()));
    connect(ui->lineEditGuarantee, SIGNAL(textChanged(QString)), this, SLOT(onJobStateHasChanged()));
    connect(ui->lineEditPrice, SIGNAL(textChanged(QString)), this, SLOT(onJobStateHasChanged()));
    connect(ui->groupBoxGuarantee, SIGNAL(toggled(bool)), this, SLOT(onJobStateHasChanged()));

    employeeWidget = new EmployeeWidget(dbConnectionsString, ui->employeeWidget);
    employeeWidget->setEmployeeCurrentId();
    ui->employeeWidget->setMinimumHeight(employeeWidget->geometry().height());
    ui->employeeWidget->setMinimumWidth(employeeWidget->geometry().width());
    adjustSize();
    //ui->groupBoxGuarantee->setEnabled(false); //temp
    employeeWidget->setEnabled(false); //temp

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
    return SetupManager::instance()->getCurrentUserId() == selectedEmployeeId;
}

QCompleter *JobListOnReceiptDialog::getCompleter()
{
    return ui->lineEditJobName->completer();
}

void JobListOnReceiptDialog::clearField()
{
    ui->lineEditJobName->clear();
    ui->lineEditPrice->clear();
    ui->spinBoxQuantity->setValue(1);    
}

void JobListOnReceiptDialog::onPushButtonAddJobClicked()
{
    if (ui->lineEditJobName->text().isEmpty() ||
            ui->lineEditPrice->text().isEmpty())
        return;

    jobsModel->addJob(m_id,
                      SetupManager::instance()->getCurrentUserId(),
                      ui->lineEditJobName->text().trimmed(),
                      ui->spinBoxQuantity->value(),
                      ui->lineEditPrice->text().toInt());
   // if (ui->lineEditJobName->property("jobId").toInt() != -1)
        jobListModel->addJob(ui->lineEditJobName->text().trimmed(),
                             ui->lineEditPrice->text().toInt(),
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
                         SetupManager::instance()->getCurrentUserId(),
                         ui->lineEditJobName->text().trimmed(),
                         ui->spinBoxQuantity->value(),
                         ui->lineEditPrice->text().toInt());
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
    ui->lineEditPrice->setText(QString::number(currentJob.price));
    ui->spinBoxQuantity->setValue(currentJob.quantity);
    ui->pushButtonDeleteJob->setEnabled(checkPermissions(currentJob.employeeId));
    ui->pushButtonUpdateJob->setEnabled(checkPermissions(currentJob.employeeId));
}

void JobListOnReceiptDialog::onCompleteJobData(QModelIndex index)
{
    qDebug() << "complete job data";
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
    ui->lineEditPrice->setText(getCompleter()->
                               completionModel()->
                               data(getCompleter()->
                                    completionModel()->
                                    index(index.row(),JobListModelHeader::Price),JobListModelData::Text).toString());
    ui->lineEditJobName->setProperty("jobId",getCompleter()->
                                     completionModel()->
                                     data(getCompleter()->
                                          completionModel()->
                                          index(index.row(),JobListModelHeader::Name),JobListModelData::ID));
}

void JobListOnReceiptDialog::onJobStateHasChanged()
{
    ui->lineEditJobName->setProperty("jobId",-1);
}

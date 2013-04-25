#include "permissionswidget.h"
#include "ui_permissionswidget.h"

#include "usersandpermissionsmanager.h"
#include <QDebug>

PermissionsWidget::PermissionsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PermissionsWidget)
{
    ui->setupUi(this);
    setEnabled(accessManager->isCanEditPermissions());

    connect(ui->pushButtonSaveChanges, SIGNAL(clicked()), this, SLOT(onSaveChangesClicked()));
}

PermissionsWidget::~PermissionsWidget()
{
    delete ui;
}

int PermissionsWidget::getCurrentPermissions() const
{
    int permissions = 0;
    qDebug() << ui->checkBoxAddTicket->checkState();
    if (ui->checkBoxAddTicket->checkState() == Qt::Checked)
        permissions |= UsersAndPermissionsManager::PAddTicket;

    if (ui->checkBoxCloseTicket->checkState() == Qt::Checked)
        permissions |= UsersAndPermissionsManager::PCloseticket;

    if (ui->checkBoxMoveToWork->checkState() == Qt::Checked)
        permissions |= UsersAndPermissionsManager::PMoveToWork;

    if (ui->checkBoxMoveToReady->checkState() == Qt::Checked)
        permissions |= UsersAndPermissionsManager::PMoveToReady;

    if (ui->checkBoxMarkTicketAsGuarantee->checkState() == Qt::Checked)
        permissions |= UsersAndPermissionsManager::PGuaranteeReturn;

    if (ui->checkBoxJobList->checkState() == Qt::Checked)
        permissions |= UsersAndPermissionsManager::PJobListEdit;

    if (ui->checkBoxBranchEdit->checkState() == Qt::Checked)
        permissions |= UsersAndPermissionsManager::PEditBranches;

    if (ui->checkBoxUsersEdit->checkState() == Qt::Checked)
        permissions |= UsersAndPermissionsManager::PEditUsers;

    if (ui->checkBoxPermissionsEdit->checkState() == Qt::Checked)
        permissions |= UsersAndPermissionsManager::PEditPermissions;

    if (ui->checkBoxEditClosed->checkState() == Qt::Checked)
        permissions |= UsersAndPermissionsManager::PEditClosedTickets;

    if (ui->checkBoxSalary->checkState() == Qt::Checked)
        permissions |= UsersAndPermissionsManager::PEditSalary;

    return permissions;
}

void PermissionsWidget::onSetPermissions(const int &value)
{
    ui->checkBoxAddTicket->setChecked(value & UsersAndPermissionsManager::PAddTicket);

    ui->checkBoxCloseTicket->setChecked(value & UsersAndPermissionsManager::PCloseticket);

    ui->checkBoxMoveToWork->setChecked(value & UsersAndPermissionsManager::PMoveToWork);

    ui->checkBoxMoveToReady->setChecked(value & UsersAndPermissionsManager::PMoveToReady);

    ui->checkBoxMarkTicketAsGuarantee->setChecked(value & UsersAndPermissionsManager::PGuaranteeReturn);

    ui->checkBoxJobList->setChecked(value & UsersAndPermissionsManager::PJobListEdit);

    ui->checkBoxBranchEdit->setChecked(value & UsersAndPermissionsManager::PEditBranches);

    ui->checkBoxUsersEdit->setChecked(value & UsersAndPermissionsManager::PEditUsers);

    ui->checkBoxPermissionsEdit->setChecked(value & UsersAndPermissionsManager::PEditPermissions);

    ui->checkBoxEditClosed->setChecked(value & UsersAndPermissionsManager::PEditClosedTickets);

    ui->checkBoxSalary->setChecked(value & UsersAndPermissionsManager::PEditSalary);
}

void PermissionsWidget::onSaveChangesClicked()
{
    qDebug() << Q_FUNC_INFO << getCurrentPermissions();
    emit setPermissions(getCurrentPermissions());
}

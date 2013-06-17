#include "employeepenaltyandbonusesdialog.h"
#include "ui_employeepenaltyandbonusesdialog.h"

#include "itemmodelforemployeepenaltyandbonuses.h"
#include "employeewidget.h"
#include "usersandpermissionsmanager.h"
#include "employeepabdialog.h"

#include <QDebug>
#include <QSpinBox>

EmployeePenaltyAndBonusesDialog::EmployeePenaltyAndBonusesDialog(const QString &dbConnectionString, QWidget *parent) :
    QDialog(parent),
    SqlExtension(dbConnectionString),
    ui(new Ui::EmployeePenaltyAndBonusesDialog)
{
    ui->setupUi(this);
    model = new ItemModelForEmployeePenaltyAndBonuses(dbConnectionString,this);
    model->getPenaltyAndBonuses();
    ui->tableViewPAB->setModel(model);
    employeeWidget = new EmployeeWidget(dbConnectionString, ui->employeeWidget);
    employeeWidget->setEmployeeCurrentId();
    ui->employeeWidget->setMinimumHeight(employeeWidget->geometry().height());
    ui->employeeWidget->setMinimumWidth(employeeWidget->geometry().width());

    ui->tableViewPAB->setItemDelegateForColumn(1,new AmountDelegate(this));

    connect(ui->pushButtonAdd, SIGNAL(clicked()), SLOT(onAddPAB()));
    connect(ui->pushButtonDelete, SIGNAL(clicked()), SLOT(onDeletePAB()));
    connect(ui->pushButtonOk, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->pushButtonCancel, SIGNAL(clicked()), SLOT(reject()));
    connect(model, SIGNAL(itemChanged(QStandardItem*)),
            this, SLOT(onItemChanged(QStandardItem*)));
    connect(employeeWidget, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentIndexChanged(int)));
    ui->tableViewPAB->resizeColumnsToContents();
    ui->groupBoxControls->setEnabled(accessManager->isCanEditSalary());
    if (!accessManager->isCanEditSalary())
        ui->tableViewPAB->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

EmployeePenaltyAndBonusesDialog::~EmployeePenaltyAndBonusesDialog()
{
    delete ui;
}

void EmployeePenaltyAndBonusesDialog::onCurrentIndexChanged(const int &current)
{
    Q_UNUSED(current);
    qDebug() << current << Q_FUNC_INFO;
    model->getPenaltyAndBonuses(getEmployeeId());
    ui->tableViewPAB->resizeColumnsToContents();
}

void EmployeePenaltyAndBonusesDialog::onItemChanged(QStandardItem *item)
{
    model->onUpdatePAB(item->index(),
                       model->item(item->index().row(),1)->text().toInt(),
                       model->item(item->index().row(),2)->text());
}

void EmployeePenaltyAndBonusesDialog::onAddPAB()
{
    EmployeePABDialog* epabd = new EmployeePABDialog(this);
    if (epabd->exec())
        model->onAddPAB(epabd->getAmount(),getEmployeeId(),epabd->getReason());
    ui->tableViewPAB->resizeColumnsToContents();
}

void EmployeePenaltyAndBonusesDialog::onDeletePAB()
{
    model->onDeletePAB(getCurrentIndex());
}

QModelIndex EmployeePenaltyAndBonusesDialog::getCurrentIndex() const
{
    return ui->tableViewPAB->currentIndex();
}

QString EmployeePenaltyAndBonusesDialog::getEmployeeId() const
{
    return employeeWidget->getCurrentEmployeeId().toString();
}


AmountDelegate::AmountDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget* AmountDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSpinBox* sb = new QSpinBox(parent);
    sb->setMaximum(9999999);
    sb->setValue(0);
    sb->setMinimum(-9999999);
    return sb;
}

void AmountDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QSpinBox *sb = qobject_cast<QSpinBox*>(editor);
    qDebug() << index << index.data() << index.data().toInt() << Q_FUNC_INFO;
    sb->setValue(index.data().toInt());
}

void AmountDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox *sb = qobject_cast<QSpinBox*>(editor);
    model->setData(index,sb->value(),Qt::DisplayRole);
}

void AmountDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect rect = option.rect;
    QSize sizeHint = editor->sizeHint();
    if (rect.width()<sizeHint.width()) rect.setWidth(sizeHint.width());
    if (rect.height()<sizeHint.height()) rect.setHeight(sizeHint.height());
    editor->setGeometry(rect);
}

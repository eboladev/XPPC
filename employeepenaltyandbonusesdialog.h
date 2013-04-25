#ifndef EMPLOYEEPENALTYANDBONUSESDIALOG_H
#define EMPLOYEEPENALTYANDBONUSESDIALOG_H

#include <QDialog>
#include <QStyledItemDelegate>
#include "sqlextension.h"

namespace Ui {
class EmployeePenaltyAndBonusesDialog;
}

class EmployeeWidget;
class ItemModelForEmployeePenaltyAndBonuses;
class QStandardItem;

class AmountDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit AmountDelegate(QObject* parent = 0);

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class EmployeePenaltyAndBonusesDialog : public QDialog, SqlExtension
{
    Q_OBJECT
    
public:
    explicit EmployeePenaltyAndBonusesDialog(const QString& dbConnectionString, QWidget *parent = 0);
    ~EmployeePenaltyAndBonusesDialog();

private Q_SLOTS:
    void onCurrentIndexChanged(const int& current);
    void onItemChanged(QStandardItem*);
    void onAddPAB();
    void onDeletePAB();

private:
    QModelIndex getCurrentIndex() const;
    QString getEmployeeId() const;
    
private:
    Ui::EmployeePenaltyAndBonusesDialog *ui;
    ItemModelForEmployeePenaltyAndBonuses* model;
    EmployeeWidget* employeeWidget;
};

#endif // EMPLOYEEPENALTYANDBONUSESDIALOG_H

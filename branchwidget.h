#ifndef BRANCHWIDGET_H
#define BRANCHWIDGET_H

#include <QDialog>
#include "sqlextension.h"

class QStandardItemModel;
class QStandardItem;
namespace Ui {
class BranchWidget;
}

class BranchWidget : public QDialog, SqlExtension
{
    Q_OBJECT
    
public:
    explicit BranchWidget(const QString &dbConnectionString,QWidget *parent = 0);
    ~BranchWidget();
private:
    void getBranches();
private slots:
    void onCurrentBrancheChanged();
    void onBranchViewChanged(QStandardItem*);
    void onAddBranchClicked();
    void onRemoveBranchClicked();
private:
    Ui::BranchWidget *ui;
    QStandardItemModel* model;
};

#endif // BRANCHWIDGET_H

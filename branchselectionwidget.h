#ifndef BRANCHSELECTIONWIDGET_H
#define BRANCHSELECTIONWIDGET_H

#include <QWidget>

namespace Ui {
class BranchSelectionWidget;
}

class BranchSelectionWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit BranchSelectionWidget(QWidget *parent = 0);
    ~BranchSelectionWidget();
    void setBranch(QVariant id);
    QVariant getCurrentBranch();

private:
    Ui::BranchSelectionWidget *ui;
};

#endif // BRANCHSELECTIONWIDGET_H

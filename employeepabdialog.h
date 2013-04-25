#ifndef EMPLOYEEPABDIALOG_H
#define EMPLOYEEPABDIALOG_H

#include <QDialog>

namespace Ui {
class EmployeePABDialog;
}

class EmployeePABDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EmployeePABDialog(QWidget *parent = 0);
    ~EmployeePABDialog();

    int getAmount();
    QString getReason();
    
private:
    Ui::EmployeePABDialog *ui;
};

#endif // EMPLOYEEPABDIALOG_H

#ifndef CONTACTDEVELOPERDIALOG_H
#define CONTACTDEVELOPERDIALOG_H

#include <QDialog>

namespace Ui {
class ContactDeveloperDialog;
}

class ContactDeveloperDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ContactDeveloperDialog(QWidget *parent = 0);
    ~ContactDeveloperDialog();

private Q_SLOTS:    
    void onSettingsClicked();
    void onAccept();
    
private:
    Ui::ContactDeveloperDialog *ui;
};

#endif // CONTACTDEVELOPERDIALOG_H

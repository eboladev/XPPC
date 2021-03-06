#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>

namespace Ui {
class ConnectDialog;
}

class ConnectDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ConnectDialog(QWidget *parent = 0);
    ~ConnectDialog();
public Q_SLOTS:
    void saveSettings();
private Q_SLOTS:
    void on_pushButtonTestConnection_clicked();

    void on_pushButtonConnect_clicked();

private:
    static void encryptDecrypt(QByteArray &ba);
    Ui::ConnectDialog *ui;
    void loadSettings();
};

#endif // CONNECTDIALOG_H

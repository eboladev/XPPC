#ifndef BARCODESCANNERSETTINGSDIALOG_H
#define BARCODESCANNERSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class BarcodeScannerSettingsDialog;
}

class BarcodeScannerSettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit BarcodeScannerSettingsDialog(QWidget *parent = 0);
    ~BarcodeScannerSettingsDialog();

private Q_SLOTS:
    void onAccept();
    void onCheckSettings();

private:
    void loadSettigns();

private:
    Ui::BarcodeScannerSettingsDialog *ui;
};

#endif // BARCODESCANNERSETTINGSDIALOG_H

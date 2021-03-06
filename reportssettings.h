#ifndef REPORTSSETTINGS_H
#define REPORTSSETTINGS_H

#include <QDialog>
#include <QMap>
namespace Ui {
class ReportsSettings;
}

class ReportSelectionWidget;

class ReportsSettings : public QDialog
{
    Q_OBJECT
    
public:
    explicit ReportsSettings(QWidget *parent = 0);
    ~ReportsSettings();

private Q_SLOTS:
    void onAccept();
    void onTicketSelect();
    void onJobSelect();
    void onCashSelect();
private:
    void fillReportSelectWidgets();
    bool executeFileDialog(QString& result);
    
private:
    Ui::ReportsSettings *ui;
    QMap<int,ReportSelectionWidget*> reportsMap;
};

#endif // REPORTSSETTINGS_H

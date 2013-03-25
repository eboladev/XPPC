#ifndef REPORTSSETTINGS_H
#define REPORTSSETTINGS_H

#include <QDialog>
#include "sqlextension.h"

namespace Ui {
class ReportsSettings;
}

class ReportSelectionWidget;

class ReportsSettings : public QDialog, SqlExtension
{
    Q_OBJECT
    
public:
    explicit ReportsSettings(const QString& dbConenctionString, QWidget *parent = 0);
    ~ReportsSettings();

private slots:
    void onAccept();

private:
    void fillReportSelectWidgets();
    
private:
    Ui::ReportsSettings *ui;
    QMap<int,ReportSelectionWidget*> reportsMap;
};

#endif // REPORTSSETTINGS_H

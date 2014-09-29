#ifndef REPORTSHANDLER_H
#define REPORTSHANDLER_H

#include <QObject>

#ifdef Q_OS_WIN32
#include "DocumentGenerators/wordautomation.h"
#include "DocumentGenerators/writerautomation.h"
#endif

struct DeviceInfo {
    QString name;
    QString serial;
    QString problem;
    QString defect;
};

class ReportsHandler : public QObject
{
    Q_OBJECT
public:
    explicit ReportsHandler(QObject *parent = 0);
    static bool openReport(const int& ticket_id, const int& report_type);
    static bool saveTicketReports(const QString &path, const int& ticket_id);        

private:
    static QString money(double n);
    static QString getTemplateType(const int& template_type);
    static QString getTemplatePath(const int& report_type);    
#ifdef Q_OS_WIN32
    static void PopulateDeviceInfoData(WordAutomation &wa, QList<DeviceInfo> deviceInfoList, int &currentRow);
    static bool loadTemplate(WordAutomation &wa, const int& report_type);
    static bool generateTicketReport(WordAutomation &wa, const int& ticket_id);
    static bool generateJobObTicketReport(WordAutomation &wa, const int& ticket_id);
    static bool generateCashReceiptReport(WordAutomation &wa, const int& ticket_id);
    //static bool generateTicketReport(WriterAutomation &wa, const QString &scenarioLogId);
#endif
    
};



#endif // REPORTSHANDLER_H

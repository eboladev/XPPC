#ifndef REPORTSHANDLER_H
#define REPORTSHANDLER_H

#include <QObject>

#ifdef Q_OS_WIN32
#include "DocumentGenerators/wordautomation.h"
#include "DocumentGenerators/writerautomation.h"
#endif

class ReportsHandler : public QObject
{
    Q_OBJECT
public:
    explicit ReportsHandler(QObject *parent = 0);
    static bool openTicketReports(const int& ticket_id);
    static bool saveTicketReports(const QString &path, const int& ticket_id);    

private:    
    QString money(double n);
    static QString getTemplateType(const int& ticket_id);
#ifdef Q_OS_WIN32
    static bool generateTicketReport(WordAutomation &wa, const int& ticket_id);
    //static bool generateTicketReport(WriterAutomation &wa, const QString &scenarioLogId);
#endif
    
};

#endif // REPORTSHANDLER_H

#include "reportshandler.h"
#include "setupmanager.h"

ReportsHandler::ReportsHandler(QObject *parent) :
    QObject(parent)
{
}

bool ReportsHandler::openTicketReports(const int &ticket_id)
{
#ifdef Q_OS_WIN32
    bool res = false;
    QString templateType = getTemplateType(ticket_id);
    /*if (templateType.contains(".ott"))
    {
        WriterAutomation wa;
        if (generateReport(wa, scenarioLogId,dbConnectionString))
        {
            wa.setVisible(true);
            res = true;
        }
    }
    else*/
        if (templateType.contains(".dot"))
        {
            WordAutomation wa;
            if (generateTicketReport(wa, ticket_id))
            {
                wa.setQuitFromWordAutomaticaly(false);
                wa.setVisible(true);
                res = true;
            }
            else
                wa.setQuitFromWordAutomaticaly(true);
        }
    return res;
#endif
}

bool ReportsHandler::saveTicketReports(const QString &path, const int &ticket_id)
{
#ifdef Q_OS_WIN32

    bool res = false;

    QString templateType = getTemplateType(ticket_id);

    /*if (templateType.contains(".ott"))
    {
        WriterAutomation wa;
        if (generateReport(wa, scenarioLogId, dbConnectionString))
        {
            wa.saveToFile(path.toStdWString());
            res = true;
        }
        if (!wa.closeDocument())
            qDebug() << "error while closing document";
        return res;
    }
    else*/
        if (templateType.contains(".dot"))
        {
            WordAutomation wa;
            if (generateTicketReport(wa, ticket_id))
            {
                wa.setQuitFromWordAutomaticaly(false);
                wa.saveToFile(path);
                res = true;
            }
            wa.quit();
            wa.setQuitFromWordAutomaticaly(true);
        }
    return res;
#endif
}

QString ReportsHandler::getTemplateType(const int &ticket_id)
{
    return  "ticket.dotx";
}

bool ReportsHandler::generateTicketReport(WordAutomation &wa, const int &ticket_id)
{
    QSqlQuery q;
    if (!SetupManager::instance()->getSqlQueryForDB(q))
        return false;
    wa.setQuitFromWordAutomaticaly(true);
    if (!wa.isWordPresent())
        return false;
    q.prepare("SELECT "
              "client.name, "
              "client.phone, "
              "device.name, "
              "device.problem, "
              "device.condition, "
              "ticket.ticket_id "
              "FROM "
              "ticket "
              "INNER JOIN client ON (ticket.client_id = client.id) "
              "INNER JOIN device ON (ticket.device_id = device.id) "
              "where ticket.id = ?");
    q.addBindValue(ticket_id);
    q.exec();
    if (!q.next())
        return false;
    QString clientName(q.value(0).toString());

    q.prepare("select "
              "document_templates.template_data "
              "from document_templates "
              "inner join scenarios on (document_templates.category_uuid = scenarios.scenaries_category_uuid) "
              "where scenarios.scenario_uuid = ?");
    /*q.addBindValue(ticket_id);
    q.exec();
    if (!q.next())
        return false;
    QString templateName = QDir::tempPath() + "/templateXXXXXX.dot";
    templateName = QDir::toNativeSeparators(templateName);
    QTemporaryFile tempFile(templateName);
    if (!tempFile.open())
        return false;
    tempFile.write(q.value(0).toByteArray());
    tempFile.close();
    if (!wa.loadDocument(tempFile.fileName(), true))
        return false;

    // статическая информация
    if (wa.findBookmarkByName("m1"))
        wa.insertText(QObject::trUtf8("Идентификатор"));
    if (wa.findBookmarkByName("m2"))
        wa.insertText(startTime.toString());
    if (wa.findBookmarkByName("m3"))
        wa.insertText(QObject::trUtf8("%0 (%1)").arg(userName).arg(userGroupName));
    if (wa.findBookmarkByName("m4"))
        wa.insertText(reportStatus == 1 ?  QObject::trUtf8("Закрыт") : QObject::trUtf8("Открыт"));
    if (wa.findBookmarkByName("m5"))
        wa.insertText(reportStatus == 0 ? QObject::trUtf8("В процессе") : completeTime.toString());
    if (wa.findBookmarkByName("m6"))
        wa.insertText(eventName);
    if (wa.findBookmarkByName("m7"))
        wa.insertText(eventInitiatorName);
    if (wa.findBookmarkByName("m8"))
        wa.insertText(QDateTime::currentDateTime().toString());*/
}

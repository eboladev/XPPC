#include "reportshandler.h"
#include "setupmanager.h"
#include "globals.h"

#include <QTemporaryFile>
#include <QDir>
#include <QDebug>
#include <QSettings>
#include <QDateTime>

ReportsHandler::ReportsHandler(QObject *parent) :
    QObject(parent)
{
}

bool ReportsHandler::openReport(const int &ticket_id, const int &report_type)
{
#ifdef Q_OS_WIN32
    bool res = false;
    qDebug() << report_type;
    QString templateType = getTemplateType(report_type);
    qDebug() << templateType;
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
            qDebug() << "template ok";
            WordAutomation wa;
            switch(report_type)
            {
            case TicketReport:
            {
                if (generateTicketReport(wa, ticket_id))
                {
                    wa.setQuitFromWordAutomaticaly(false);
                    wa.setVisible(true);
                    res = true;
                }
                else
                    wa.setQuitFromWordAutomaticaly(true);
            }
                break;
            case JobListReport:
            {
                if (generateJobObTicketReport(wa, ticket_id))
                {
                    wa.setQuitFromWordAutomaticaly(false);
                    wa.setVisible(true);
                    res = true;
                }
                else
                    wa.setQuitFromWordAutomaticaly(true);
            }
                break;
            case CashReceiptReport:
            {
                if (generateCashReceiptReport(wa, ticket_id))
                {
                    wa.setQuitFromWordAutomaticaly(false);
                    wa.setVisible(true);
                    res = true;
                }
                else
                    wa.setQuitFromWordAutomaticaly(true);
            }
                break;
            case PricetTagReport:
            {

            }
                break;
            case CashCheckReport:
            {

            }
                break;
            }

        }
    return res;
#endif
}

bool ReportsHandler::saveTicketReports(const QString &path, const int &ticket_id)
{
#ifdef Q_OS_WIN32

    bool res = false;

    QString templateType;// = getTemplateType(ticket_id);

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

QString ReportsHandler::money(double n)
{
        if ( n > 999999999999.99 )
            return QString::fromUtf8("Очень много денег ...");

        if ( n < 0 )
            return QString::fromUtf8("Отрицательная сумма !");

        QString cap[4][10] =
        {
            {
                "",
                QString::fromUtf8("один "),
                QString::fromUtf8("два "),
                QString::fromUtf8("три "),
                QString::fromUtf8("четыре "),
                QString::fromUtf8("пять "),
                QString::fromUtf8("шесть "),
                QString::fromUtf8("семь "),
                QString::fromUtf8("восемь "),
                QString::fromUtf8("девять ")
            },
            {
                "",
                QString::fromUtf8("одна "),
                QString::fromUtf8("две "),
                QString::fromUtf8("три "),
                QString::fromUtf8("четыре "),
                QString::fromUtf8("пять "),
                QString::fromUtf8("шесть "),
                QString::fromUtf8("семь "),
                QString::fromUtf8("восемь "),
                QString::fromUtf8("девять ")
            },
            {
                "",
                "",
                QString::fromUtf8("двадцать "),
                QString::fromUtf8("тридцать "),
                QString::fromUtf8("сорок "),
                QString::fromUtf8("пятьдесят "),
                QString::fromUtf8("шестьдесят "),
                QString::fromUtf8("семьдесят "),
                QString::fromUtf8("восемьдесят "),
                QString::fromUtf8("девяносто ")
            },
            {
                "",
                QString::fromUtf8("сто "),
                QString::fromUtf8("двести "),
                QString::fromUtf8("триста "),
                QString::fromUtf8("четыреста "),
                QString::fromUtf8("пятьсот "),
                QString::fromUtf8("шестьсот "),
                QString::fromUtf8("семьсот "),
                QString::fromUtf8("восемьсот "),
                QString::fromUtf8("девятьсот ")
            }
        };

        QString cap2[10] =
        {
            QString::fromUtf8("десять "),
            QString::fromUtf8("одиннадцать "),
            QString::fromUtf8("двенадцать "),
            QString::fromUtf8("тринадцать "),
            QString::fromUtf8("четырнадцать "),
            QString::fromUtf8("пятнадцать "),
            QString::fromUtf8("шестнадцать "),
            QString::fromUtf8("семнадцать "),
            QString::fromUtf8("восемнадцать "),
            QString::fromUtf8("девятнадцать ")
        };

        QString cap3[5][3] =
        {
            {
                QString::fromUtf8("копейка "),
                QString::fromUtf8("копейки "),
                QString::fromUtf8("копеек ")
            },
            {
                QString::fromUtf8("рубль, "),
                QString::fromUtf8("рубля, "),
                QString::fromUtf8("рублей, ")
            },
            {
                QString::fromUtf8("тысяча "),
                QString::fromUtf8("тысячи "),
                QString::fromUtf8("тысяч ")
            },
            {
                QString::fromUtf8("миллион "),
                QString::fromUtf8("милиона "),
                QString::fromUtf8("миллионов ")
            },
            {
                QString::fromUtf8("миллиард "),
                QString::fromUtf8("миллиарда "),
                QString::fromUtf8("миллиардов ")
            }
        };

        QString s;

        qlonglong nn = ((qlonglong)(n)*10000 + (qlonglong)(n*1000) % 1000 + 5 ) / 10;

        qlonglong lo = 0;
        qlonglong hi = nn;
        int r[3];
        int rod;

        for ( int nPor = 0; true; ++nPor )
        {
            QString olds = s;
            s = "";

            lo = hi%1000;
            hi = hi/1000;

            if ((nPor == 1) && (lo == 0) && (hi == 0))
            {
                s = QString::fromUtf8("ноль ") + cap3[1][2] + olds;
                break;
            }

            for ( int nRaz = 0; nRaz < 3; ++nRaz )
            {
                r[nRaz] = lo%10;
                lo      = lo/10;
            }

            switch ( nPor )
            {
                case 0:
                case 1:
                case 2:
                    rod = 1;
                    break;
                default:
                    rod = 0;
            }

            QString capt;
            if (r[1] != 1)
            {
                if (r[0] == 1)
                    capt = cap3[nPor][0];
                else if ((r[0] > 1 ) && (r[0] < 5))
                    capt = cap3[nPor][1];
            }
            if (capt.isEmpty())
                capt = cap3[nPor][2];

            if (nPor == 0)
            {
                s = QString::number(r[1]) + QString::number(r[0]) + " " + capt;
                continue;
            }
            if (nPor == 1)
                rod = 0;

            s += cap[2+1][r[2]];
            s += ((r[1] == 1) ? cap2[r[0]] : cap[1+1][r[1]]+cap[0+rod][r[0]])+capt;
            s += olds;

            if ((hi == 0) && (nPor >= 1))
                break;
        }
        QString up(s[0]);
        s.replace(0,1,up);
        return s;
}

QString ReportsHandler::getTemplateType(const int &template_type)
{
    QRegExp re;
    re.setPattern("[//*\\\\*](\\w*\\d*\\.\\w*)");
    if (re.indexIn(getTemplatePath(template_type),0) >= 0)
        return re.cap(1); //filename;
    else
        return "";
}

QString ReportsHandler::getTemplatePath(const int &report_type)
{
    QSettings s;
    return s.value(QString("reports/%0").arg(QString::number(report_type))).toString();
}

bool ReportsHandler::loadTemplate(WordAutomation &wa, const int &report_type)
{
    QString templateName = QDir::tempPath() + "/templateXXXXXX.dot";
    templateName = QDir::toNativeSeparators(templateName);

    qDebug() << templateName;

    QTemporaryFile tempFile(templateName);
    if (!tempFile.open())
        return false;

    qDebug() << "temp file opened";

    QFile repDot(getTemplatePath(report_type));

    qDebug() << "rep path" << getTemplatePath(report_type);

    if (!repDot.open(QIODevice::ReadOnly))
    {
        qDebug() << "template didnt open";
        return false;
    }

    qDebug() << "report opened, and its size:" << repDot.size();

    tempFile.write(repDot.readAll());    
    repDot.close();
    qDebug() << "document loaded ?" << tempFile.fileName() << "size" << tempFile.size();

    tempFile.close();

    if (!wa.loadDocument(tempFile.fileName(),true))
        return false;
    else
        return true;
}

#ifdef Q_OS_WIN32

void ReportsHandler::PopulateDeviceInfoData(WordAutomation &wa, QList<DeviceInfo> deviceInfoList, int& currentRow)
{
    foreach (DeviceInfo di, deviceInfoList) {
        wa.insertRows(1);
        //row number
        if (wa.selectCellInTableByPos(1,currentRow))
            wa.insertText(QString::number(currentRow-1));
        //name
        if (wa.selectCellInTableByPos(2,currentRow))
            wa.insertText(di.name);
        //serial
        if (wa.selectCellInTableByPos(3,currentRow))
            wa.insertText(di.serial);
        //defects
        if (wa.selectCellInTableByPos(4,currentRow))
            wa.insertText(di.defect);
        //problem
        if (wa.selectCellInTableByPos(5,currentRow))
            wa.insertText(di.problem);
        ++currentRow;
    }
}

bool ReportsHandler::generateTicketReport(WordAutomation &wa, const int &ticket_id)
{        
    QSqlQuery q;

    if (!SetupManager::instance()->getSqlQueryForDB(q))
        return false;
    qDebug() << "db connect ok" << Q_FUNC_INFO;
    wa.setQuitFromWordAutomaticaly(true);

    qDebug() << wa.isWordPresent() << "word present" << Q_FUNC_INFO;

    if (!wa.isWordPresent())
        return false;

    q.prepare("SELECT "
              "client.name, "
              "client.phone, "
              "ticket.ticket_id "              
              "FROM "
              "client_ticket "
              "INNER JOIN client ON (client_ticket.client_id = client.id) "
              "INNER JOIN ticket ON (ticket.ticket_id = client_ticket.id) "
              "INNER JOIN device ON (device.id = ticket.device_id) "
              "where ticket.id = ?");
    q.addBindValue(ticket_id);

    if (!q.exec())
    {
        qDebug() << q.lastError() << q.lastQuery();        
        return false;
    }
    qDebug() << "query ok";
    if (!q.next())
        return false;
    qDebug() << "query have data";

    QString clientName(q.value(0).toString());
    QString clientPhone(q.value(1).toString());
    QString ticketId(q.value(2).toString());

    qDebug() << "b4 loading template for Tickets";

    if (!loadTemplate(wa,TicketReport))
        return false;

    qDebug() << "template loaded";

    QString currentTime = QDateTime::currentDateTime().toString("dd-MM-yyyy");

    if (wa.findBookmarkByName("client_name"))
        wa.insertText(clientName);
    if (wa.findBookmarkByName("client_phone"))
        wa.insertText(clientPhone);
    if (wa.findBookmarkByName("ticket_number"))
        wa.insertText(ticketId);
    if (wa.findBookmarkByName("current_date"))
        wa.insertText(currentTime);

    if (wa.findBookmarkByName("client_name2"))
        wa.insertText(clientName);
    if (wa.findBookmarkByName("client_phone2"))
        wa.insertText(clientPhone);
    if (wa.findBookmarkByName("ticket_number2"))
        wa.insertText(ticketId);
    if (wa.findBookmarkByName("current_date2"))
        wa.insertText(currentTime);

    QSqlQuery q2;

    if (!SetupManager::instance()->getSqlQueryForDB(q2))
        return false;
    qDebug() << "db connect ok" << Q_FUNC_INFO;

    q2.prepare("SELECT "
              "device.name, "
              "device.SERIAL,"
              "device.condition, "//defects
              "device.problem "
               "FROM "
              "client_ticket "
              "INNER JOIN client ON (client_ticket.client_id = client.id) "
              "INNER JOIN ticket ON (ticket.ticket_id = client_ticket.id) "
              "INNER JOIN device ON (device.id = ticket.device_id) "
              "where ticket.ticket_id = ?");
    q2.addBindValue(ticketId);

    if (!q2.exec())
    {
        qDebug() << q2.lastError() << q2.lastQuery();
        return false;
    }
    qDebug() << "query ok";

    QList<DeviceInfo> deviceInfoList;
    while (q2.next())
    {
        DeviceInfo di;
        di.name = q2.value(0).toString();
        di.serial = q2.value(1).toString();
        di.defect = q2.value(2).toString();
        di.problem = q2.value(3).toString();
        deviceInfoList.append(di);
    }

    if (wa.findTableByName("deviceTable"))
    {
        int currentRow = 2;
        PopulateDeviceInfoData(wa, deviceInfoList, currentRow);

        if (wa.findTableByName("deviceTable2")) {
            currentRow = 2;
            PopulateDeviceInfoData(wa, deviceInfoList, currentRow);
        }
    } else {
        if (deviceInfoList.isEmpty()) {
            qDebug() << "legacy mode failed, device info list is empty" << Q_FUNC_INFO;
            return false;
        }
        DeviceInfo di = deviceInfoList.first();
        //for legacy reports
        if (wa.findBookmarkByName("device_condition"))
            wa.insertText(di.defect);
        if (wa.findBookmarkByName("device_name"))
            wa.insertText(di.name);
        if (wa.findBookmarkByName("device_problem"))
            wa.insertText(di.problem);
        if (wa.findBookmarkByName("device_serial"))
            wa.insertText(di.serial);
        if (wa.findBookmarkByName("device_condition2"))
            wa.insertText(di.defect);
        if (wa.findBookmarkByName("device_name2"))
            wa.insertText(di.name);
        if (wa.findBookmarkByName("device_problem2"))
            wa.insertText(di.problem);
        if (wa.findBookmarkByName("device_serial2"))
            wa.insertText(di.serial);
    }
    return true;
}

bool ReportsHandler::generateJobObTicketReport(WordAutomation &wa, const int &ticket_id)
{
    QSqlQuery q;

    if (!SetupManager::instance()->getSqlQueryForDB(q))
        return false;

    wa.setQuitFromWordAutomaticaly(true);

    if (!wa.isWordPresent())
        return false;

    if (!loadTemplate(wa,JobListReport))
        return false;

    if (wa.findTableByName("jobTable"))
    {
        q.prepare("select job_name,job_quantity,job_price from JobOnTicket "
                  " where tdc_r_id=?");
        q.addBindValue(ticket_id);
        if (!q.exec())
        {
            qDebug() << q.lastError() << q.lastQuery();
            return false;
        }

        int row = 1;
        int currentRow = 2;
        int totalPrice = 0;
        while (q.next())
        {
            wa.insertRows(1);
            if (wa.selectCellInTableByPos(1,currentRow))
                wa.insertText(QString::number(row));
            if (wa.selectCellInTableByPos(2,currentRow))
                wa.insertText(q.value(0).toString());
            if (wa.selectCellInTableByPos(3,currentRow))
                wa.insertText(trUtf8("шт."));
            if (wa.selectCellInTableByPos(4,currentRow))
                wa.insertText(q.value(1).toString());
            if (wa.selectCellInTableByPos(5,currentRow))
                wa.insertText(q.value(2).toString());
            if (wa.selectCellInTableByPos(6,currentRow))
                wa.insertText(QString::number(q.value(1).toInt()*q.value(2).toInt()));
            totalPrice += q.value(1).toInt()*q.value(2).toInt();
            ++row;
            ++currentRow;
        }
        wa.insertRows(1);
        if (wa.selectCellInTableByPos(2,currentRow))
            wa.insertText(trUtf8("Итого:"));
        if (wa.selectCellInTableByPos(6,currentRow))
            wa.insertText(QString::number(totalPrice));
        if (wa.findBookmarkByName("jobs_total"))
            wa.insertText(QString::number(row-1));
        if (wa.findBookmarkByName("current_date"))
            wa.insertText(QDate::currentDate().toString("dd.MM.yyyy"));
        if (wa.findBookmarkByName("current_date2"))
            wa.insertText(QDate::currentDate().toString("dd.MM.yyyy"));

        q.prepare("select client.name, client_ticket.id "
                  "FROM "
                  "client_ticket "
                  "INNER JOIN client ON (client_ticket.client_id = client.id) "
                  "INNER JOIN ticket ON (ticket.ticket_id = client_ticket.id) where ticket.id = ?");
        q.addBindValue(ticket_id);
        q.exec();
        if (q.next())
        {
            if (wa.findBookmarkByName("client_name"))
                wa.insertText(q.value(0).toString());
            if (wa.findBookmarkByName("client_name2"))
                wa.insertText(q.value(0).toString());
            if (wa.findBookmarkByName("ticket_id"))
                wa.insertText(q.value(1).toString());
            if (wa.findBookmarkByName("ticket_id2"))
                wa.insertText(q.value(1).toString());
        }
        if (wa.findBookmarkByName("total_price"))
            wa.insertText(money(totalPrice));
        return true;
    }
}

bool ReportsHandler::generateCashReceiptReport(WordAutomation &wa, const int &ticket_id)
{
    QSqlQuery q;

    if (!SetupManager::instance()->getSqlQueryForDB(q))
        return false;

    qDebug() << "db connect ok" << Q_FUNC_INFO;

    wa.setQuitFromWordAutomaticaly(true);

    qDebug() << wa.isWordPresent() << "word present" << Q_FUNC_INFO;

    if (!wa.isWordPresent())
        return false;

    q.prepare("SELECT "
              "client.name, "
              "device.price, "
              "ticket.ticket_id "
              "FROM "
              "client_ticket "
              "INNER JOIN client ON (client_ticket.client_id = client.id) "
              "INNER JOIN ticket ON (ticket.ticket_id = client_ticket.id) "
              "INNER JOIN device ON (device.id = ticket.device_id) "
              "where ticket.id = ?");
    q.addBindValue(ticket_id);

    if (!q.exec())
    {
        qDebug() << q.lastError() << q.lastQuery();
        return false;
    }
    qDebug() << "query ok";
    if (!q.next())
        return false;
    qDebug() << "query have data";
    QString clientName(q.value(0).toString());
    int devicePrice(q.value(1).toInt());
    QString ticketId(q.value(2).toString());

    if (!loadTemplate(wa,CashReceiptReport))
        return false;

    qDebug() << "template loaded";

    if (wa.findBookmarkByName("client_name"))
        wa.insertText(clientName);
    if (wa.findBookmarkByName("client_name2"))
        wa.insertText(clientName);
    if (wa.findBookmarkByName("price"))
        wa.insertText(QString::number(devicePrice));
    if (wa.findBookmarkByName("price2"))
        wa.insertText(QString::number(devicePrice));
    if (wa.findBookmarkByName("ticket_number"))
        wa.insertText(ticketId);
    if (wa.findBookmarkByName("ticket_number2"))
        wa.insertText(ticketId);
    if (wa.findBookmarkByName("date"))
        wa.insertText(QDateTime::currentDateTime().toString("dd.MM.yyyy"));
    if (wa.findBookmarkByName("date2"))
        wa.insertText(QDateTime::currentDateTime().toString("\"dd\" MMMM yyyy г."));
    if (wa.findBookmarkByName("date3"))
        wa.insertText(QDateTime::currentDateTime().toString("\"dd\" MMMM yyyy г."));
    if (wa.findBookmarkByName("price_in_words"))
        wa.insertText(money(devicePrice));
    if (wa.findBookmarkByName("price_in_words2"))
        wa.insertText(money(devicePrice));
    return true;
}
#endif

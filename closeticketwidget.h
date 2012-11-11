#ifndef CLOSETICKETWIDGET_H
#define CLOSETICKETWIDGET_H

#include <QDialog>
#include "sqlextension.h"

class QSqlQueryModel;

namespace Ui {
class CloseTicketWidget;
}

class CloseTicketWidget : public QDialog, SqlExtension
{
    Q_OBJECT
    
public:
    explicit CloseTicketWidget(const QString &dbConnectionName,const int &id, QWidget *parent = 0);
    ~CloseTicketWidget();
private:
    void getJobs(const int &id);
    void formPrice();
private slots:
    void closeTicket();
private:
    Ui::CloseTicketWidget *ui;
    QSqlQueryModel* model;
    int m_id;
};

#endif // CLOSETICKETWIDGET_H

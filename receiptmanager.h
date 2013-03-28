#ifndef RECEIPTMANAGER_H
#define RECEIPTMANAGER_H

#include <QDialog>
#include "sqlextension.h"

namespace Ui {
class ReceiptManager;
}

struct Ticket
{
    int tId; //ticket id
    int ctId;
    int cId; // client id
    QString cName;
    QString cPhone;
    int dId; //device id
    QString dName;
    QString dCond;
    QString dSN;
    QString dProblem;
    int bId; // branch id
};

class ReceiptManager : public QDialog, SqlExtension
{
    Q_OBJECT
    
public:    
    explicit ReceiptManager(const QString dbConnectionsString, const int& id = -1, QWidget *parent = 0);
    ~ReceiptManager();
    
private slots:
    void clearFields();
    void onAccept();

private:
    void setupConnections();    
    void fillFields(int id);    

private:
    Ui::ReceiptManager *ui;
    QList<int> deviceIdList;
    int currentTicketId;
    bool editMode;
};

#endif // RECEIPTMANAGER_H

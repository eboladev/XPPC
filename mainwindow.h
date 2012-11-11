#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QSqlQueryModel;
class QStatusBar;
class QModelIndex;
class QTimer;
class QNetworkConfiguration;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    enum TicketStatus
    {
        Ready,
        InWork,        
        Closed
    };

private slots:
    void onAddReceiptClicked();
    void onJobListClicked();
    void onSettingsClicked();

    void on_radioButtonReady_pressed();
    void on_radioButtonWorking_pressed();
    void on_radioButtonClosed_pressed();

    void on_tableViewTicket_clicked(const QModelIndex &index);

    void on_pushButtonSearchClear_clicked();
    void on_pushButtonSearch_clicked();

    void on_radioButtonClosed_toggled(bool checked);
    void on_radioButtonWorking_toggled(bool checked);
    void on_radioButtonReady_toggled(bool checked);

    void on_tableViewTicket_doubleClicked(const QModelIndex &index);

    void on_actionDisconnect_triggered();
    void on_actionConnect_triggered();

    QString formTicketQuery(int ticketStatus,int limit);

    void makeUpdate();

    void networkFuckedUpTwo(const QNetworkConfiguration &);

    void on_actionPrintTicket_triggered();
    void on_actionBranches_triggered();
    void on_actionCloseTicket_triggered();
private:
    void fillTicketViewModel(QString query);
    bool checkDbConnection();
    bool checkDbSettings();
    bool connectToDb(QString dbConnectionName);
    bool disconnectFromDb(QString dbConnectionName);
    bool settingsIsNotEmpty();
    void sb(QString text);
    void genReport(const int &type);
private:        
    Ui::MainWindow *ui;
    QSqlQueryModel* model;
    QTimer* updateTableViewTicket;
    QString defaultConfName;
    int currentStatus;
    int currentTicket;
};

#endif // MAINWINDOW_H

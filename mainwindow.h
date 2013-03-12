#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

class QSqlQueryModel;
class QStatusBar;
class QTimer;
class QNetworkConfiguration;
class QSortFilterProxyModel;
class QStandardItemModel;
class QAction;
class ChangeUserDialog;
class QStandardItem;

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
    void onTableViewTicketSelectionChanged(QModelIndex, QModelIndex);
    void onCustomContextMenuRequested(const QPoint &pos);

    void onMoveBackToWork();
    void onMoveBackToReady();

    void on_actionDisconnect_triggered();
    void on_actionConnect_triggered();

    void makeUpdate();

    void on_actionPrintTicket_triggered();
    void onActionBranchesTriggered();
    void on_actionCloseTicket_triggered();
    void onActionChangeUserClicked();
    void onActionUserManagementClicked();    
    void onChangeUserInPopupMenu();
    void onRejectUserInPopupMenu();
    void changePermissions();

    void onTabChanged(int);
    void onRefreshCategoryModel();
    void onRefreshProductByType(int type);
    void onCurrentCategoryChanged(QModelIndex, QModelIndex);
    void onActionCategoryProductsClicked();

private:
    void fillTicketViewModel(QString query);
    bool checkDbConnection();
    bool checkDbSettings();
    bool connectToDb(QString dbConnectionName);
    bool disconnectFromDb(QString dbConnectionName);
    bool settingsIsNotEmpty();
    void sb(QString text);
    void genReport(const int &type);        
    bool changeUser(const QString& login, const QString& password);
    QString formTicketQuery(int ticketStatus,int limit);
    QVariant getCurrentTicketId();

signals:
    void refreshProductModelByCategory(int);

private:
    Ui::MainWindow *ui;
    QSqlQueryModel* ticketModel;
    QStandardItemModel* jobModel;
    QStandardItemModel* proCatModel; //product category model
    QSqlQueryModel* productModel;
    QSortFilterProxyModel* proxyProduct;
    ChangeUserDialog* cud;
    QTimer* updateTableViewTicket;
    QString defaultConfName;
    QSortFilterProxyModel* proxy;
    int currentStatus;
    int currentTicket;
};

#endif // MAINWINDOW_H

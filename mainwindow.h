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

    enum Tabs
    {
        Tickets = 0,
        Merchandise
    };

    enum TicketStatus
    {
        InWork,
        Ready,                
        Closed
    };

    /*base begin*/
private slots:
    void onActionChangeUserClicked();
    void onActionBranchesTriggered();
    void onActionUserManagementClicked();
    void onChangeUserInPopupMenu();
    void onRejectUserInPopupMenu();
    void changePermissions();
    void on_actionDisconnect_triggered();
    void on_actionConnect_triggered();
    void onTabChanged(int);
private:
    bool checkDbConnection();
    bool checkDbSettings();
    bool connectToDb(QString dbConnectionName);
    bool disconnectFromDb(QString dbConnectionName);
    bool settingsIsNotEmpty();
    void sb(QString text);    
    bool changeUser(const QString& login, const QString& password);
    /*base end*/

    /*ticket manager start*/
private slots:
    void onAddTicketClicked();
    void onJobListClicked();

    void onSettingsClicked();

    void on_radioButtonReady_pressed();
    void on_radioButtonWorking_pressed();
    void on_radioButtonClosed_pressed();

    void refreshTicketModel(const QString&);

    void on_pushButtonSearchClear_clicked();
    void on_pushButtonSearch_clicked();

    void on_tableViewTicket_doubleClicked(const QModelIndex &index);
    void onTableViewTicketSelectionChanged(QModelIndex, QModelIndex);
    void onCustomContextMenuRequested(const QPoint &pos);

    void onMoveBackToWork();
    void onMoveBackToReady();

    void onQueryLimitComboBoxIndexChanged(int);

    void on_actionCloseTicket_triggered();

private:
    QString generateTicketQuery();
    //ticket device client relation id
    QVariant getCurrentTDCRId();
    int getCurrentTicketId();
    /*ticket manager stop*/

    /* Merchandise manager begin */
private slots:
    void onRefreshCategoryModel();
    void onRefreshProductByType(int type);
    void onCurrentCategoryChanged(QModelIndex, QModelIndex);
    void onActionCategoryProductsClicked();  

signals:
    void refreshProductModelByCategory(int);
    /* Merchandise manager end */

private:
    Ui::MainWindow *ui;
    QStandardItemModel* ticketModel;
    QSortFilterProxyModel* ticketProxy;
    QStandardItemModel* jobModel;
    QTimer* updateTableViewTicket;
    int currentStatus;

    QStandardItemModel* proCatModel; //product category model
    QSqlQueryModel* productModel;
    QSortFilterProxyModel* proxyProduct;

    ChangeUserDialog* cud;    
    QString defaultConfName;            
};

#endif // MAINWINDOW_H

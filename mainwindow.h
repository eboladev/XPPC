#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

#include "globals.h"

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

    /*base begin*/
public Q_SLOTS:
    void onUserLogOut();
    void onUserLogIn();
    void onFailedToLogin(const QString&);

private Q_SLOTS:
    void onActionChangeUserClicked();
    void onActionBranchesTriggered();
    void onActionUserManagementClicked();
    void onActionReportSettignsClicked();
    void onActionSMSGatewaysSettingsClicked();
    void changePermissions();
    void on_actionDisconnect_triggered();
    void on_actionConnect_triggered();    
    bool eventFilter(QObject *, QEvent *);
    void serUserDisplayName();
    void onActionDeveloperContactClicked();
    void onActionSmsTemplateSettingsClicked();

private:
    bool checkDbConnection();
    bool checkDbSettings();
    bool connectToDb(QString dbConnectionName);
    bool disconnectFromDb(QString dbConnectionName);
    bool settingsIsNotEmpty();
    void sb(QString text);

    void initAccessManager();

    /*base end*/

    /*ticket manager start*/
private Q_SLOTS:    
    void onAddTicketClicked();
    void onJobListClicked();

    void onDBSettingsClicked();

    void on_radioButtonReady_pressed();
    void on_radioButtonWorking_pressed();
    void on_radioButtonClosed_pressed();

    void refreshTicketModel(const QString&);

    void on_pushButtonSearchClear_clicked();
    void on_pushButtonSearch_clicked();

    void onShowCommentsTabClicked();
    void onAddCommentToTicketClicked();
    void onRemoveCommentClicked();

    void onGenerateTicketReport();
    void onGenerateJobListReport();
    void onGenerateCashReceiptReport();

    void onNotAGuaranteeClicked();
    void onAcceptAGuaranteeClicked();

    void on_tableViewTicket_doubleClicked(const QModelIndex &index);
    void onTableViewTicketSelectionChanged(QModelIndex, QModelIndex);
    void onCustomContextMenuRequested(const QPoint &pos);
    void onCommentsCustomContextMenuRequested(const QPoint &pos);
    void onIsClientNotifiedClicked(const QModelIndex &index);

    void onCloseTicketClicked();
    void onMoveBackToWork();
    void onMoveBackToReady();
    void submitGuaranteeTicket();

    void onSendTicketNotifySmsClicked();

    void onQueryLimitComboBoxIndexChanged(int);

    void onSmsSended(const int& ticket_id);
    void onSmsDelivered(const int& ticket_id);
    void onSmsInProcess(const int& id);
    void onSmsNonDelivered(const int& id);
    void onBalance(const double& currency);

private:
    void onSetGuaranteeDone();
    QString generateTicketQuery(const bool& search = false);
    //ticket device client relation id
    QVariant getCurrentTDCRId();
    QVariant getCurrentTDCRId(const QModelIndex& index);
    void changeTicketStatus(const int& status);
    int getCurrentTicketId();
    int getCurrentTicketId(const QModelIndex& index);
    QString getCurrentTicketDeviceName(const QModelIndex& index);
    QString getCurrentTicketDevicePrice();
    QString getCurrentTicketDeviceSerial(const QModelIndex& index);
    QString getCurrentTicketPhones(const QModelIndex& index);
    QString getCurrentTicketData(const TicketColumns& column,
                                 const TicketRoles& role = static_cast<TicketRoles>(Qt::DisplayRole));
    QVariant getCurrentTicketGuaranteeId(const QModelIndex& index);
    void updateSmsWidget(const int& id,const int& ticket_id = -1);
    QString genTicketNotifySms();
    /*ticket manager stop*/

    /* Merchandise manager begin */
private Q_SLOTS:
    void onRefreshCategoryModel();
    void onRefreshProductByType(int type);
    void onCurrentCategoryChanged(QModelIndex, QModelIndex);
    void onActionCategoryProductsClicked();
    void onTicketNotificationSmsTextChanged();
    void onTicketPriceChanged(QString);

Q_SIGNALS:
    void refreshProductModelByCategory(int);
    void disconnectedFromDb();
    /* Merchandise manager end */

private:
    Ui::MainWindow *ui;
    QStandardItemModel* ticketModel;
    QSortFilterProxyModel* ticketProxy;
    QStandardItemModel* ticketComments;
    QStandardItemModel* jobModel;
    QTimer* updateTableViewTicket;

    int currentStatus;

    bool connectEstablished;

    QStandardItemModel* proCatModel; //product category model
    QSqlQueryModel* productModel;
    QSortFilterProxyModel* proxyProduct;

    QString defaultConfName;            
};

#endif // MAINWINDOW_H

/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sun 11. Nov 15:04:25 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTableView>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionConnect;
    QAction *actionDisconnect;
    QAction *actionSettingsMenuClicked;
    QAction *actionExitMenuClicked;
    QAction *actionOnAddReceiptClicked;
    QAction *actionOnJobListClicked;
    QAction *actionCloseTicket;
    QAction *actionPrintTicket;
    QAction *action_12;
    QAction *action_13;
    QAction *actionBranchTriggered;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_4;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBoxSortReceipt;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_4;
    QRadioButton *radioButtonReady;
    QRadioButton *radioButtonWorking;
    QRadioButton *radioButtonClosed;
    QGroupBox *groupBocSearchReceipt;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButtonSearch;
    QPushButton *pushButtonSearchClear;
    QTableView *tableViewTicket;
    QWidget *tab_2;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 611);
        MainWindow->setMinimumSize(QSize(800, 600));
        actionConnect = new QAction(MainWindow);
        actionConnect->setObjectName(QString::fromUtf8("actionConnect"));
        actionDisconnect = new QAction(MainWindow);
        actionDisconnect->setObjectName(QString::fromUtf8("actionDisconnect"));
        actionSettingsMenuClicked = new QAction(MainWindow);
        actionSettingsMenuClicked->setObjectName(QString::fromUtf8("actionSettingsMenuClicked"));
        actionExitMenuClicked = new QAction(MainWindow);
        actionExitMenuClicked->setObjectName(QString::fromUtf8("actionExitMenuClicked"));
        actionOnAddReceiptClicked = new QAction(MainWindow);
        actionOnAddReceiptClicked->setObjectName(QString::fromUtf8("actionOnAddReceiptClicked"));
        actionOnJobListClicked = new QAction(MainWindow);
        actionOnJobListClicked->setObjectName(QString::fromUtf8("actionOnJobListClicked"));
        actionCloseTicket = new QAction(MainWindow);
        actionCloseTicket->setObjectName(QString::fromUtf8("actionCloseTicket"));
        actionPrintTicket = new QAction(MainWindow);
        actionPrintTicket->setObjectName(QString::fromUtf8("actionPrintTicket"));
        action_12 = new QAction(MainWindow);
        action_12->setObjectName(QString::fromUtf8("action_12"));
        action_13 = new QAction(MainWindow);
        action_13->setObjectName(QString::fromUtf8("action_13"));
        actionBranchTriggered = new QAction(MainWindow);
        actionBranchTriggered->setObjectName(QString::fromUtf8("actionBranchTriggered"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(1);
        gridLayout->setContentsMargins(2, 2, 2, 2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setMovable(true);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_4 = new QGridLayout(tab);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBoxSortReceipt = new QGroupBox(tab);
        groupBoxSortReceipt->setObjectName(QString::fromUtf8("groupBoxSortReceipt"));
        gridLayout_3 = new QGridLayout(groupBoxSortReceipt);
        gridLayout_3->setSpacing(1);
        gridLayout_3->setContentsMargins(2, 2, 2, 2);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        radioButtonReady = new QRadioButton(groupBoxSortReceipt);
        radioButtonReady->setObjectName(QString::fromUtf8("radioButtonReady"));

        horizontalLayout_4->addWidget(radioButtonReady);

        radioButtonWorking = new QRadioButton(groupBoxSortReceipt);
        radioButtonWorking->setObjectName(QString::fromUtf8("radioButtonWorking"));
        radioButtonWorking->setChecked(true);

        horizontalLayout_4->addWidget(radioButtonWorking);

        radioButtonClosed = new QRadioButton(groupBoxSortReceipt);
        radioButtonClosed->setObjectName(QString::fromUtf8("radioButtonClosed"));

        horizontalLayout_4->addWidget(radioButtonClosed);


        gridLayout_3->addLayout(horizontalLayout_4, 0, 0, 1, 1);


        horizontalLayout->addWidget(groupBoxSortReceipt);

        groupBocSearchReceipt = new QGroupBox(tab);
        groupBocSearchReceipt->setObjectName(QString::fromUtf8("groupBocSearchReceipt"));
        gridLayout_2 = new QGridLayout(groupBocSearchReceipt);
        gridLayout_2->setSpacing(1);
        gridLayout_2->setContentsMargins(2, 2, 2, 2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(1);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        lineEdit = new QLineEdit(groupBocSearchReceipt);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        verticalLayout->addWidget(lineEdit);


        horizontalLayout_3->addLayout(verticalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(1);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pushButtonSearch = new QPushButton(groupBocSearchReceipt);
        pushButtonSearch->setObjectName(QString::fromUtf8("pushButtonSearch"));
        pushButtonSearch->setAutoDefault(true);
        pushButtonSearch->setDefault(true);

        horizontalLayout_2->addWidget(pushButtonSearch);

        pushButtonSearchClear = new QPushButton(groupBocSearchReceipt);
        pushButtonSearchClear->setObjectName(QString::fromUtf8("pushButtonSearchClear"));

        horizontalLayout_2->addWidget(pushButtonSearchClear);


        horizontalLayout_3->addLayout(horizontalLayout_2);


        gridLayout_2->addLayout(horizontalLayout_3, 0, 0, 1, 1);


        horizontalLayout->addWidget(groupBocSearchReceipt);


        gridLayout_4->addLayout(horizontalLayout, 0, 0, 1, 1);

        tableViewTicket = new QTableView(tab);
        tableViewTicket->setObjectName(QString::fromUtf8("tableViewTicket"));
        tableViewTicket->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        tableViewTicket->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableViewTicket->setAlternatingRowColors(true);
        tableViewTicket->setSelectionMode(QAbstractItemView::SingleSelection);
        tableViewTicket->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableViewTicket->setSortingEnabled(false);
        tableViewTicket->horizontalHeader()->setDefaultSectionSize(50);
        tableViewTicket->horizontalHeader()->setMinimumSectionSize(10);
        tableViewTicket->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        tableViewTicket->horizontalHeader()->setStretchLastSection(true);
        tableViewTicket->verticalHeader()->setVisible(false);
        tableViewTicket->verticalHeader()->setDefaultSectionSize(20);
        tableViewTicket->verticalHeader()->setMinimumSectionSize(10);

        gridLayout_4->addWidget(tableViewTicket, 1, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tabWidget->addTab(tab_2, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 21));
        menuBar->setDefaultUp(false);
        menuBar->setNativeMenuBar(true);
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_2 = new QMenu(menu);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        menu_3 = new QMenu(menuBar);
        menu_3->setObjectName(QString::fromUtf8("menu_3"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        QWidget::setTabOrder(lineEdit, pushButtonSearch);
        QWidget::setTabOrder(pushButtonSearch, pushButtonSearchClear);
        QWidget::setTabOrder(pushButtonSearchClear, radioButtonReady);
        QWidget::setTabOrder(radioButtonReady, radioButtonWorking);
        QWidget::setTabOrder(radioButtonWorking, radioButtonClosed);
        QWidget::setTabOrder(radioButtonClosed, tableViewTicket);
        QWidget::setTabOrder(tableViewTicket, tabWidget);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_3->menuAction());
        menuFile->addAction(actionConnect);
        menuFile->addAction(actionDisconnect);
        menuFile->addSeparator();
        menuFile->addAction(actionSettingsMenuClicked);
        menuFile->addSeparator();
        menuFile->addAction(actionExitMenuClicked);
        menu->addAction(actionOnAddReceiptClicked);
        menu->addAction(actionOnJobListClicked);
        menu->addAction(actionCloseTicket);
        menu->addAction(menu_2->menuAction());
        menu_2->addAction(actionPrintTicket);
        menu_2->addAction(action_12);
        menu_2->addAction(action_13);
        menu_3->addAction(actionBranchTriggered);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Service centre manager", 0, QApplication::UnicodeUTF8));
        actionConnect->setText(QApplication::translate("MainWindow", "\320\237\320\276\320\264\320\272\320\273\321\216\321\207\320\270\321\202\321\214\321\201\321\217", 0, QApplication::UnicodeUTF8));
        actionDisconnect->setText(QApplication::translate("MainWindow", "\320\236\321\202\320\272\320\273\321\216\321\207\320\270\321\202\321\214\321\201\321\217", 0, QApplication::UnicodeUTF8));
        actionSettingsMenuClicked->setText(QApplication::translate("MainWindow", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", 0, QApplication::UnicodeUTF8));
        actionExitMenuClicked->setText(QApplication::translate("MainWindow", "\320\222\321\213\321\205\320\276\320\264", 0, QApplication::UnicodeUTF8));
        actionOnAddReceiptClicked->setText(QApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        actionOnJobListClicked->setText(QApplication::translate("MainWindow", "\320\241\320\277\320\270\321\201\320\276\320\272 \321\200\320\260\320\261\320\276\321\202", 0, QApplication::UnicodeUTF8));
        actionCloseTicket->setText(QApplication::translate("MainWindow", "\320\227\320\260\320\272\321\200\321\213\321\202\321\214", 0, QApplication::UnicodeUTF8));
        actionPrintTicket->setText(QApplication::translate("MainWindow", "\320\232\320\262\320\270\321\202\320\260\320\275\321\206\320\270\321\217", 0, QApplication::UnicodeUTF8));
        action_12->setText(QApplication::translate("MainWindow", "\320\220\320\272\321\202 \320\262\321\213\320\277\320\276\320\273\320\275\320\265\320\275\320\275\321\213\321\205 \321\200\320\260\320\261\320\276\321\202", 0, QApplication::UnicodeUTF8));
        action_13->setText(QApplication::translate("MainWindow", "\320\237\321\200\320\270\321\205\320\276\320\264\320\275\321\213\320\271 \320\272\320\260\321\201\321\201\320\276\320\262\321\213\320\271 \320\276\321\200\320\264\320\265\321\200", 0, QApplication::UnicodeUTF8));
        actionBranchTriggered->setText(QApplication::translate("MainWindow", "\320\244\320\270\320\273\320\270\320\260\320\273\321\213", 0, QApplication::UnicodeUTF8));
        groupBoxSortReceipt->setTitle(QApplication::translate("MainWindow", "\320\241\320\276\321\200\321\202\320\270\321\200\320\276\320\262\320\272\320\260 \320\272\320\262\320\270\321\202\320\260\320\275\321\206\320\270\320\271", 0, QApplication::UnicodeUTF8));
        radioButtonReady->setText(QApplication::translate("MainWindow", "\320\223\320\276\321\202\320\276\320\262\321\213\320\265(\320\275\320\260 \320\262\321\213\320\264\320\260\321\207\320\265)", 0, QApplication::UnicodeUTF8));
        radioButtonWorking->setText(QApplication::translate("MainWindow", "\320\222 \321\200\320\260\320\261\320\276\321\202\320\265", 0, QApplication::UnicodeUTF8));
        radioButtonClosed->setText(QApplication::translate("MainWindow", "\320\227\320\260\320\272\321\200\321\213\321\202\321\213\320\265", 0, QApplication::UnicodeUTF8));
        groupBocSearchReceipt->setTitle(QApplication::translate("MainWindow", "\320\237\320\276\320\270\321\201\320\272", 0, QApplication::UnicodeUTF8));
        pushButtonSearch->setText(QApplication::translate("MainWindow", "\320\237\320\276\320\270\321\201\320\272", 0, QApplication::UnicodeUTF8));
        pushButtonSearchClear->setText(QApplication::translate("MainWindow", "\320\236\321\207\320\270\321\201\321\202\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "\320\232\320\262\320\270\321\202\320\260\320\275\321\206\320\270\320\270", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Tab 2", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menu->setTitle(QApplication::translate("MainWindow", "\320\232\320\262\320\270\321\202\320\260\320\275\321\206\320\270\320\270", 0, QApplication::UnicodeUTF8));
        menu_2->setTitle(QApplication::translate("MainWindow", "\320\237\320\265\321\207\320\260\321\202\321\214", 0, QApplication::UnicodeUTF8));
        menu_3->setTitle(QApplication::translate("MainWindow", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

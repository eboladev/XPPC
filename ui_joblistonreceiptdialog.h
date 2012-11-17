/********************************************************************************
** Form generated from reading UI file 'joblistonreceiptdialog.ui'
**
** Created: Sat 17. Nov 16:36:32 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_JOBLISTONRECEIPTDIALOG_H
#define UI_JOBLISTONRECEIPTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableView>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_JobListOnReceiptDialog
{
public:
    QGridLayout *gridLayout_3;
    QVBoxLayout *verticalLayout_7;
    QGroupBox *groupBoxMainInformation;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_6;
    QVBoxLayout *verticalLayout;
    QLabel *labelEmployeeList;
    QComboBox *comboBoxEmployeeList;
    QVBoxLayout *verticalLayout_2;
    QLabel *labelJobName;
    QLineEdit *lineEditJobName;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_3;
    QLabel *labelQuantity;
    QLineEdit *lineEditQuantity;
    QVBoxLayout *verticalLayout_4;
    QLabel *labelPrice;
    QLineEdit *lineEditPrice;
    QGroupBox *groupBoxJobOperations;
    QGridLayout *gridLayout_2;
    QPushButton *pushButtonAddJob;
    QPushButton *pushButtonUpdateJob;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonDeleteJob;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButtonClearField;
    QTableView *tableView;
    QFrame *line;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButtonOK;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *pushButtonCheckReady;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButtonCancel;

    void setupUi(QDialog *JobListOnReceiptDialog)
    {
        if (JobListOnReceiptDialog->objectName().isEmpty())
            JobListOnReceiptDialog->setObjectName(QString::fromUtf8("JobListOnReceiptDialog"));
        JobListOnReceiptDialog->resize(500, 400);
        JobListOnReceiptDialog->setMinimumSize(QSize(500, 400));
        gridLayout_3 = new QGridLayout(JobListOnReceiptDialog);
        gridLayout_3->setSpacing(1);
        gridLayout_3->setContentsMargins(2, 2, 2, 2);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        groupBoxMainInformation = new QGroupBox(JobListOnReceiptDialog);
        groupBoxMainInformation->setObjectName(QString::fromUtf8("groupBoxMainInformation"));
        gridLayout = new QGridLayout(groupBoxMainInformation);
        gridLayout->setSpacing(1);
        gridLayout->setContentsMargins(2, 2, 2, 2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(1);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(1);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelEmployeeList = new QLabel(groupBoxMainInformation);
        labelEmployeeList->setObjectName(QString::fromUtf8("labelEmployeeList"));

        verticalLayout->addWidget(labelEmployeeList);

        comboBoxEmployeeList = new QComboBox(groupBoxMainInformation);
        comboBoxEmployeeList->setObjectName(QString::fromUtf8("comboBoxEmployeeList"));

        verticalLayout->addWidget(comboBoxEmployeeList);


        verticalLayout_6->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        labelJobName = new QLabel(groupBoxMainInformation);
        labelJobName->setObjectName(QString::fromUtf8("labelJobName"));

        verticalLayout_2->addWidget(labelJobName);

        lineEditJobName = new QLineEdit(groupBoxMainInformation);
        lineEditJobName->setObjectName(QString::fromUtf8("lineEditJobName"));

        verticalLayout_2->addWidget(lineEditJobName);


        verticalLayout_6->addLayout(verticalLayout_2);


        horizontalLayout->addLayout(verticalLayout_6);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(1);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(1);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        labelQuantity = new QLabel(groupBoxMainInformation);
        labelQuantity->setObjectName(QString::fromUtf8("labelQuantity"));

        verticalLayout_3->addWidget(labelQuantity);

        lineEditQuantity = new QLineEdit(groupBoxMainInformation);
        lineEditQuantity->setObjectName(QString::fromUtf8("lineEditQuantity"));

        verticalLayout_3->addWidget(lineEditQuantity);


        verticalLayout_5->addLayout(verticalLayout_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(1);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        labelPrice = new QLabel(groupBoxMainInformation);
        labelPrice->setObjectName(QString::fromUtf8("labelPrice"));

        verticalLayout_4->addWidget(labelPrice);

        lineEditPrice = new QLineEdit(groupBoxMainInformation);
        lineEditPrice->setObjectName(QString::fromUtf8("lineEditPrice"));

        verticalLayout_4->addWidget(lineEditPrice);


        verticalLayout_5->addLayout(verticalLayout_4);


        horizontalLayout->addLayout(verticalLayout_5);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        verticalLayout_7->addWidget(groupBoxMainInformation);

        groupBoxJobOperations = new QGroupBox(JobListOnReceiptDialog);
        groupBoxJobOperations->setObjectName(QString::fromUtf8("groupBoxJobOperations"));
        gridLayout_2 = new QGridLayout(groupBoxJobOperations);
        gridLayout_2->setSpacing(1);
        gridLayout_2->setContentsMargins(2, 2, 2, 2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        pushButtonAddJob = new QPushButton(groupBoxJobOperations);
        pushButtonAddJob->setObjectName(QString::fromUtf8("pushButtonAddJob"));

        gridLayout_2->addWidget(pushButtonAddJob, 0, 0, 1, 1);

        pushButtonUpdateJob = new QPushButton(groupBoxJobOperations);
        pushButtonUpdateJob->setObjectName(QString::fromUtf8("pushButtonUpdateJob"));

        gridLayout_2->addWidget(pushButtonUpdateJob, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(84, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 4, 1, 1);

        pushButtonDeleteJob = new QPushButton(groupBoxJobOperations);
        pushButtonDeleteJob->setObjectName(QString::fromUtf8("pushButtonDeleteJob"));

        gridLayout_2->addWidget(pushButtonDeleteJob, 0, 5, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_3, 0, 2, 1, 1);

        pushButtonClearField = new QPushButton(groupBoxJobOperations);
        pushButtonClearField->setObjectName(QString::fromUtf8("pushButtonClearField"));

        gridLayout_2->addWidget(pushButtonClearField, 0, 3, 1, 1);


        verticalLayout_7->addWidget(groupBoxJobOperations);


        gridLayout_3->addLayout(verticalLayout_7, 0, 0, 1, 1);

        tableView = new QTableView(JobListOnReceiptDialog);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableView->setEditTriggers(QAbstractItemView::DoubleClicked);
        tableView->setAlternatingRowColors(true);
        tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->setShowGrid(true);
        tableView->horizontalHeader()->setDefaultSectionSize(70);
        tableView->horizontalHeader()->setMinimumSectionSize(10);
        tableView->horizontalHeader()->setStretchLastSection(true);
        tableView->verticalHeader()->setVisible(false);

        gridLayout_3->addWidget(tableView, 1, 0, 1, 1);

        line = new QFrame(JobListOnReceiptDialog);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(line, 2, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pushButtonOK = new QPushButton(JobListOnReceiptDialog);
        pushButtonOK->setObjectName(QString::fromUtf8("pushButtonOK"));

        horizontalLayout_2->addWidget(pushButtonOK);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        pushButtonCheckReady = new QPushButton(JobListOnReceiptDialog);
        pushButtonCheckReady->setObjectName(QString::fromUtf8("pushButtonCheckReady"));

        horizontalLayout_2->addWidget(pushButtonCheckReady);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        pushButtonCancel = new QPushButton(JobListOnReceiptDialog);
        pushButtonCancel->setObjectName(QString::fromUtf8("pushButtonCancel"));

        horizontalLayout_2->addWidget(pushButtonCancel);


        gridLayout_3->addLayout(horizontalLayout_2, 3, 0, 1, 1);

#ifndef QT_NO_SHORTCUT
        labelEmployeeList->setBuddy(comboBoxEmployeeList);
        labelJobName->setBuddy(lineEditJobName);
        labelQuantity->setBuddy(lineEditQuantity);
        labelPrice->setBuddy(lineEditPrice);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(comboBoxEmployeeList, lineEditJobName);
        QWidget::setTabOrder(lineEditJobName, lineEditQuantity);
        QWidget::setTabOrder(lineEditQuantity, lineEditPrice);
        QWidget::setTabOrder(lineEditPrice, pushButtonAddJob);
        QWidget::setTabOrder(pushButtonAddJob, pushButtonOK);
        QWidget::setTabOrder(pushButtonOK, pushButtonUpdateJob);
        QWidget::setTabOrder(pushButtonUpdateJob, tableView);
        QWidget::setTabOrder(tableView, pushButtonCancel);
        QWidget::setTabOrder(pushButtonCancel, pushButtonDeleteJob);

        retranslateUi(JobListOnReceiptDialog);

        QMetaObject::connectSlotsByName(JobListOnReceiptDialog);
    } // setupUi

    void retranslateUi(QDialog *JobListOnReceiptDialog)
    {
        JobListOnReceiptDialog->setWindowTitle(QApplication::translate("JobListOnReceiptDialog", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265 \321\201\320\277\320\270\321\201\320\272\320\276\320\274 \321\200\320\260\320\261\320\276\321\202", 0, QApplication::UnicodeUTF8));
        groupBoxMainInformation->setTitle(QApplication::translate("JobListOnReceiptDialog", "\320\224\320\260\320\275\320\275\321\213\320\265 \320\277\320\276 \321\200\320\260\320\261\320\276\321\202\320\265:", 0, QApplication::UnicodeUTF8));
        labelEmployeeList->setText(QApplication::translate("JobListOnReceiptDialog", "\320\241\320\277\320\270\321\201\320\276\320\272 \321\201\320\276\321\202\321\200\321\203\320\264\320\275\320\270\320\272\320\276\320\262:", 0, QApplication::UnicodeUTF8));
        labelJobName->setText(QApplication::translate("JobListOnReceiptDialog", "\320\235\320\260\320\270\320\274\320\265\320\275\320\276\320\262\320\260\320\275\320\270\320\265 \321\200\320\260\320\261\320\276\321\202\321\213:", 0, QApplication::UnicodeUTF8));
        labelQuantity->setText(QApplication::translate("JobListOnReceiptDialog", "\320\232\320\276\320\273\320\270\321\207\320\265\321\201\321\202\320\262\320\276:", 0, QApplication::UnicodeUTF8));
        lineEditQuantity->setText(QApplication::translate("JobListOnReceiptDialog", "1", 0, QApplication::UnicodeUTF8));
        labelPrice->setText(QApplication::translate("JobListOnReceiptDialog", "\320\246\320\265\320\275\320\260:", 0, QApplication::UnicodeUTF8));
        groupBoxJobOperations->setTitle(QApplication::translate("JobListOnReceiptDialog", "\320\236\320\277\320\265\321\200\320\260\321\206\320\270\320\270 \321\201 \321\200\320\260\320\261\320\276\321\202\320\260\320\274\320\270:", 0, QApplication::UnicodeUTF8));
        pushButtonAddJob->setText(QApplication::translate("JobListOnReceiptDialog", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        pushButtonUpdateJob->setText(QApplication::translate("JobListOnReceiptDialog", "\320\236\320\261\320\275\320\276\320\262\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        pushButtonDeleteJob->setText(QApplication::translate("JobListOnReceiptDialog", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        pushButtonClearField->setText(QApplication::translate("JobListOnReceiptDialog", "\320\236\321\207\320\270\321\201\321\202\320\270\321\202\321\214 \320\277\320\276\320\273\321\217", 0, QApplication::UnicodeUTF8));
        pushButtonOK->setText(QApplication::translate("JobListOnReceiptDialog", "Ok", 0, QApplication::UnicodeUTF8));
        pushButtonCheckReady->setText(QApplication::translate("JobListOnReceiptDialog", "\320\232\320\262\320\270\321\202\320\260\320\275\321\206\320\270\321\217 \320\262\321\213\320\277\320\276\320\273\320\275\320\265\320\275\320\260", 0, QApplication::UnicodeUTF8));
        pushButtonCancel->setText(QApplication::translate("JobListOnReceiptDialog", "\320\236\321\202\320\274\320\265\320\275\320\260", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class JobListOnReceiptDialog: public Ui_JobListOnReceiptDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JOBLISTONRECEIPTDIALOG_H

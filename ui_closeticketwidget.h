/********************************************************************************
** Form generated from reading UI file 'closeticketwidget.ui'
**
** Created: Sun 18. Nov 13:52:44 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLOSETICKETWIDGET_H
#define UI_CLOSETICKETWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CloseTicketWidget
{
public:
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *labelPrice;
    QLineEdit *lineEditPrice;
    QGroupBox *groupBoxJobsOnTicket;
    QGridLayout *gridLayout;
    QTreeView *treeViewJobsOnTIcket;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonOk;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonPrint;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButtonClose;

    void setupUi(QWidget *CloseTicketWidget)
    {
        if (CloseTicketWidget->objectName().isEmpty())
            CloseTicketWidget->setObjectName(QString::fromUtf8("CloseTicketWidget"));
        CloseTicketWidget->resize(619, 397);
        CloseTicketWidget->setMinimumSize(QSize(619, 397));
        gridLayout_2 = new QGridLayout(CloseTicketWidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelPrice = new QLabel(CloseTicketWidget);
        labelPrice->setObjectName(QString::fromUtf8("labelPrice"));

        verticalLayout->addWidget(labelPrice);

        lineEditPrice = new QLineEdit(CloseTicketWidget);
        lineEditPrice->setObjectName(QString::fromUtf8("lineEditPrice"));

        verticalLayout->addWidget(lineEditPrice);


        gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);

        groupBoxJobsOnTicket = new QGroupBox(CloseTicketWidget);
        groupBoxJobsOnTicket->setObjectName(QString::fromUtf8("groupBoxJobsOnTicket"));
        gridLayout = new QGridLayout(groupBoxJobsOnTicket);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        treeViewJobsOnTIcket = new QTreeView(groupBoxJobsOnTicket);
        treeViewJobsOnTIcket->setObjectName(QString::fromUtf8("treeViewJobsOnTIcket"));

        gridLayout->addWidget(treeViewJobsOnTIcket, 1, 0, 1, 1);


        gridLayout_2->addWidget(groupBoxJobsOnTicket, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButtonOk = new QPushButton(CloseTicketWidget);
        pushButtonOk->setObjectName(QString::fromUtf8("pushButtonOk"));

        horizontalLayout->addWidget(pushButtonOk);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButtonPrint = new QPushButton(CloseTicketWidget);
        pushButtonPrint->setObjectName(QString::fromUtf8("pushButtonPrint"));

        horizontalLayout->addWidget(pushButtonPrint);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        pushButtonClose = new QPushButton(CloseTicketWidget);
        pushButtonClose->setObjectName(QString::fromUtf8("pushButtonClose"));

        horizontalLayout->addWidget(pushButtonClose);


        gridLayout_2->addLayout(horizontalLayout, 2, 0, 1, 1);


        retranslateUi(CloseTicketWidget);

        QMetaObject::connectSlotsByName(CloseTicketWidget);
    } // setupUi

    void retranslateUi(QWidget *CloseTicketWidget)
    {
        CloseTicketWidget->setWindowTitle(QApplication::translate("CloseTicketWidget", "Form", 0, QApplication::UnicodeUTF8));
        labelPrice->setText(QApplication::translate("CloseTicketWidget", "\320\230\321\202\320\276\320\263\320\276:", 0, QApplication::UnicodeUTF8));
        groupBoxJobsOnTicket->setTitle(QApplication::translate("CloseTicketWidget", "\320\241\320\277\320\270\321\201\320\276\320\272 \320\262\321\213\320\277\320\276\320\273\320\275\320\265\320\275\320\275\321\213\321\205 \321\200\320\260\320\261\320\276\321\202:", 0, QApplication::UnicodeUTF8));
        pushButtonOk->setText(QApplication::translate("CloseTicketWidget", "\320\236\320\272", 0, QApplication::UnicodeUTF8));
        pushButtonPrint->setText(QApplication::translate("CloseTicketWidget", "\320\240\320\260\321\201\320\277\320\265\321\207\320\260\321\202\320\260\321\202\321\214 \320\260\320\272\321\202", 0, QApplication::UnicodeUTF8));
        pushButtonClose->setText(QApplication::translate("CloseTicketWidget", "\320\236\321\202\320\274\320\265\320\275\320\260", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CloseTicketWidget: public Ui_CloseTicketWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLOSETICKETWIDGET_H

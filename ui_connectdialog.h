/********************************************************************************
** Form generated from reading UI file 'connectdialog.ui'
**
** Created: Sun 11. Nov 18:41:15 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNECTDIALOG_H
#define UI_CONNECTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ConnectDialog
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *labelHost;
    QLineEdit *lineEditHost;
    QVBoxLayout *verticalLayout_5;
    QLabel *labelDBName;
    QLineEdit *lineEditDBName;
    QVBoxLayout *verticalLayout_2;
    QLabel *labelPort;
    QLineEdit *lineEditPort;
    QVBoxLayout *verticalLayout_3;
    QLabel *labelUserName;
    QLineEdit *lineEditName;
    QVBoxLayout *verticalLayout_4;
    QLabel *labelPassword;
    QLineEdit *lineEditPassword;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonConnect;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonTestConnection;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButtonClose;

    void setupUi(QDialog *ConnectDialog)
    {
        if (ConnectDialog->objectName().isEmpty())
            ConnectDialog->setObjectName(QString::fromUtf8("ConnectDialog"));
        ConnectDialog->setWindowModality(Qt::WindowModal);
        ConnectDialog->resize(400, 321);
        ConnectDialog->setMinimumSize(QSize(400, 321));
        ConnectDialog->setMaximumSize(QSize(400, 321));
        gridLayout = new QGridLayout(ConnectDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelHost = new QLabel(ConnectDialog);
        labelHost->setObjectName(QString::fromUtf8("labelHost"));

        verticalLayout->addWidget(labelHost);

        lineEditHost = new QLineEdit(ConnectDialog);
        lineEditHost->setObjectName(QString::fromUtf8("lineEditHost"));

        verticalLayout->addWidget(lineEditHost);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        labelDBName = new QLabel(ConnectDialog);
        labelDBName->setObjectName(QString::fromUtf8("labelDBName"));

        verticalLayout_5->addWidget(labelDBName);

        lineEditDBName = new QLineEdit(ConnectDialog);
        lineEditDBName->setObjectName(QString::fromUtf8("lineEditDBName"));

        verticalLayout_5->addWidget(lineEditDBName);


        gridLayout->addLayout(verticalLayout_5, 1, 0, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        labelPort = new QLabel(ConnectDialog);
        labelPort->setObjectName(QString::fromUtf8("labelPort"));

        verticalLayout_2->addWidget(labelPort);

        lineEditPort = new QLineEdit(ConnectDialog);
        lineEditPort->setObjectName(QString::fromUtf8("lineEditPort"));

        verticalLayout_2->addWidget(lineEditPort);


        gridLayout->addLayout(verticalLayout_2, 2, 0, 1, 1);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        labelUserName = new QLabel(ConnectDialog);
        labelUserName->setObjectName(QString::fromUtf8("labelUserName"));

        verticalLayout_3->addWidget(labelUserName);

        lineEditName = new QLineEdit(ConnectDialog);
        lineEditName->setObjectName(QString::fromUtf8("lineEditName"));

        verticalLayout_3->addWidget(lineEditName);


        gridLayout->addLayout(verticalLayout_3, 3, 0, 1, 1);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(2);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        labelPassword = new QLabel(ConnectDialog);
        labelPassword->setObjectName(QString::fromUtf8("labelPassword"));

        verticalLayout_4->addWidget(labelPassword);

        lineEditPassword = new QLineEdit(ConnectDialog);
        lineEditPassword->setObjectName(QString::fromUtf8("lineEditPassword"));
        lineEditPassword->setEchoMode(QLineEdit::Password);

        verticalLayout_4->addWidget(lineEditPassword);


        gridLayout->addLayout(verticalLayout_4, 4, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButtonConnect = new QPushButton(ConnectDialog);
        pushButtonConnect->setObjectName(QString::fromUtf8("pushButtonConnect"));
        pushButtonConnect->setDefault(true);

        horizontalLayout->addWidget(pushButtonConnect);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButtonTestConnection = new QPushButton(ConnectDialog);
        pushButtonTestConnection->setObjectName(QString::fromUtf8("pushButtonTestConnection"));

        horizontalLayout->addWidget(pushButtonTestConnection);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        pushButtonClose = new QPushButton(ConnectDialog);
        pushButtonClose->setObjectName(QString::fromUtf8("pushButtonClose"));

        horizontalLayout->addWidget(pushButtonClose);


        gridLayout->addLayout(horizontalLayout, 5, 0, 1, 1);

#ifndef QT_NO_SHORTCUT
        labelHost->setBuddy(lineEditHost);
        labelDBName->setBuddy(lineEditDBName);
        labelPort->setBuddy(lineEditPort);
        labelUserName->setBuddy(lineEditName);
        labelPassword->setBuddy(lineEditPassword);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(lineEditHost, lineEditDBName);
        QWidget::setTabOrder(lineEditDBName, lineEditPort);
        QWidget::setTabOrder(lineEditPort, lineEditName);
        QWidget::setTabOrder(lineEditName, lineEditPassword);
        QWidget::setTabOrder(lineEditPassword, pushButtonConnect);
        QWidget::setTabOrder(pushButtonConnect, pushButtonTestConnection);
        QWidget::setTabOrder(pushButtonTestConnection, pushButtonClose);

        retranslateUi(ConnectDialog);

        QMetaObject::connectSlotsByName(ConnectDialog);
    } // setupUi

    void retranslateUi(QDialog *ConnectDialog)
    {
        ConnectDialog->setWindowTitle(QApplication::translate("ConnectDialog", "Connection settings", 0, QApplication::UnicodeUTF8));
        labelHost->setText(QApplication::translate("ConnectDialog", "Host name\\IP address:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lineEditHost->setToolTip(QApplication::translate("ConnectDialog", "195.46.162.200", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lineEditHost->setText(QApplication::translate("ConnectDialog", "195.46.162.200", 0, QApplication::UnicodeUTF8));
        labelDBName->setText(QApplication::translate("ConnectDialog", "Database file location:", 0, QApplication::UnicodeUTF8));
        lineEditDBName->setText(QApplication::translate("ConnectDialog", "Service_test", 0, QApplication::UnicodeUTF8));
        labelPort->setText(QApplication::translate("ConnectDialog", "Port:", 0, QApplication::UnicodeUTF8));
        lineEditPort->setText(QApplication::translate("ConnectDialog", "3306", 0, QApplication::UnicodeUTF8));
        labelUserName->setText(QApplication::translate("ConnectDialog", "User name:", 0, QApplication::UnicodeUTF8));
        lineEditName->setText(QApplication::translate("ConnectDialog", "admin", 0, QApplication::UnicodeUTF8));
        labelPassword->setText(QApplication::translate("ConnectDialog", "Password:", 0, QApplication::UnicodeUTF8));
        lineEditPassword->setText(QApplication::translate("ConnectDialog", "dctvgbpltw", 0, QApplication::UnicodeUTF8));
        pushButtonConnect->setText(QApplication::translate("ConnectDialog", "Connect", 0, QApplication::UnicodeUTF8));
        pushButtonTestConnection->setText(QApplication::translate("ConnectDialog", "Test connection", 0, QApplication::UnicodeUTF8));
        pushButtonClose->setText(QApplication::translate("ConnectDialog", "Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ConnectDialog: public Ui_ConnectDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECTDIALOG_H

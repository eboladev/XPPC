/********************************************************************************
** Form generated from reading UI file 'gsmmodemmanager.ui'
**
** Created: Sun 18. Nov 14:22:41 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GSMMODEMMANAGER_H
#define UI_GSMMODEMMANAGER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeView>

QT_BEGIN_NAMESPACE

class Ui_GsmModemManager
{
public:
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonAdd;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonRemove;
    QTreeView *treeView;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButtonOk;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButtonCancel;

    void setupUi(QDialog *GsmModemManager)
    {
        if (GsmModemManager->objectName().isEmpty())
            GsmModemManager->setObjectName(QString::fromUtf8("GsmModemManager"));
        GsmModemManager->resize(294, 305);
        gridLayout_2 = new QGridLayout(GsmModemManager);
        gridLayout_2->setSpacing(2);
        gridLayout_2->setContentsMargins(3, 3, 3, 3);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        groupBox = new QGroupBox(GsmModemManager);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(2);
        gridLayout->setContentsMargins(1, 1, 1, 1);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButtonAdd = new QPushButton(groupBox);
        pushButtonAdd->setObjectName(QString::fromUtf8("pushButtonAdd"));

        horizontalLayout->addWidget(pushButtonAdd);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButtonRemove = new QPushButton(groupBox);
        pushButtonRemove->setObjectName(QString::fromUtf8("pushButtonRemove"));

        horizontalLayout->addWidget(pushButtonRemove);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);

        treeView = new QTreeView(groupBox);
        treeView->setObjectName(QString::fromUtf8("treeView"));

        gridLayout->addWidget(treeView, 0, 0, 1, 1);

        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout->addWidget(pushButton, 3, 0, 1, 1);

        pushButton_2 = new QPushButton(groupBox);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        gridLayout->addWidget(pushButton_2, 2, 0, 1, 1);


        gridLayout_2->addWidget(groupBox, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pushButtonOk = new QPushButton(GsmModemManager);
        pushButtonOk->setObjectName(QString::fromUtf8("pushButtonOk"));

        horizontalLayout_2->addWidget(pushButtonOk);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        pushButtonCancel = new QPushButton(GsmModemManager);
        pushButtonCancel->setObjectName(QString::fromUtf8("pushButtonCancel"));

        horizontalLayout_2->addWidget(pushButtonCancel);


        gridLayout_2->addLayout(horizontalLayout_2, 1, 0, 1, 1);


        retranslateUi(GsmModemManager);

        QMetaObject::connectSlotsByName(GsmModemManager);
    } // setupUi

    void retranslateUi(QDialog *GsmModemManager)
    {
        GsmModemManager->setWindowTitle(QApplication::translate("GsmModemManager", "Dialog", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("GsmModemManager", "modems", 0, QApplication::UnicodeUTF8));
        pushButtonAdd->setText(QApplication::translate("GsmModemManager", "add", 0, QApplication::UnicodeUTF8));
        pushButtonRemove->setText(QApplication::translate("GsmModemManager", "del", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("GsmModemManager", "PushButton", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("GsmModemManager", "PushButton", 0, QApplication::UnicodeUTF8));
        pushButtonOk->setText(QApplication::translate("GsmModemManager", "ok", 0, QApplication::UnicodeUTF8));
        pushButtonCancel->setText(QApplication::translate("GsmModemManager", "cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GsmModemManager: public Ui_GsmModemManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GSMMODEMMANAGER_H

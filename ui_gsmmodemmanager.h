/********************************************************************************
** Form generated from reading UI file 'gsmmodemmanager.ui'
**
** Created: Sat 17. Nov 16:36:32 2012
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
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_2;
    QTreeView *treeView;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_3;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_4;

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
        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout->addWidget(pushButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_2 = new QPushButton(groupBox);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);

        treeView = new QTreeView(groupBox);
        treeView->setObjectName(QString::fromUtf8("treeView"));

        gridLayout->addWidget(treeView, 0, 0, 1, 1);


        gridLayout_2->addWidget(groupBox, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pushButton_3 = new QPushButton(GsmModemManager);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

        horizontalLayout_2->addWidget(pushButton_3);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        pushButton_4 = new QPushButton(GsmModemManager);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));

        horizontalLayout_2->addWidget(pushButton_4);


        gridLayout_2->addLayout(horizontalLayout_2, 1, 0, 1, 1);


        retranslateUi(GsmModemManager);

        QMetaObject::connectSlotsByName(GsmModemManager);
    } // setupUi

    void retranslateUi(QDialog *GsmModemManager)
    {
        GsmModemManager->setWindowTitle(QApplication::translate("GsmModemManager", "Dialog", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("GsmModemManager", "modems", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("GsmModemManager", "add", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("GsmModemManager", "del", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("GsmModemManager", "ok", 0, QApplication::UnicodeUTF8));
        pushButton_4->setText(QApplication::translate("GsmModemManager", "cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GsmModemManager: public Ui_GsmModemManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GSMMODEMMANAGER_H

/********************************************************************************
** Form generated from reading UI file 'branchwidget.ui'
**
** Created: Sun 11. Nov 13:47:45 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BRANCHWIDGET_H
#define UI_BRANCHWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BranchWidget
{
public:
    QGridLayout *gridLayout_3;
    QGroupBox *groupBoxCurrentBranch;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    QComboBox *comboBoxCurrentBranch;
    QGroupBox *groupBoxBranchManager;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButtonAddBranch;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButtonRemoveBranch;
    QTreeView *treeViewBranch;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonOk;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonCancel;

    void setupUi(QWidget *BranchWidget)
    {
        if (BranchWidget->objectName().isEmpty())
            BranchWidget->setObjectName(QString::fromUtf8("BranchWidget"));
        BranchWidget->resize(294, 366);
        gridLayout_3 = new QGridLayout(BranchWidget);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        groupBoxCurrentBranch = new QGroupBox(BranchWidget);
        groupBoxCurrentBranch->setObjectName(QString::fromUtf8("groupBoxCurrentBranch"));
        gridLayout_2 = new QGridLayout(groupBoxCurrentBranch);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        comboBoxCurrentBranch = new QComboBox(groupBoxCurrentBranch);
        comboBoxCurrentBranch->setObjectName(QString::fromUtf8("comboBoxCurrentBranch"));

        verticalLayout->addWidget(comboBoxCurrentBranch);


        gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);


        gridLayout_3->addWidget(groupBoxCurrentBranch, 0, 0, 1, 1);

        groupBoxBranchManager = new QGroupBox(BranchWidget);
        groupBoxBranchManager->setObjectName(QString::fromUtf8("groupBoxBranchManager"));
        gridLayout = new QGridLayout(groupBoxBranchManager);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pushButtonAddBranch = new QPushButton(groupBoxBranchManager);
        pushButtonAddBranch->setObjectName(QString::fromUtf8("pushButtonAddBranch"));

        horizontalLayout_2->addWidget(pushButtonAddBranch);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        pushButtonRemoveBranch = new QPushButton(groupBoxBranchManager);
        pushButtonRemoveBranch->setObjectName(QString::fromUtf8("pushButtonRemoveBranch"));

        horizontalLayout_2->addWidget(pushButtonRemoveBranch);


        gridLayout->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        treeViewBranch = new QTreeView(groupBoxBranchManager);
        treeViewBranch->setObjectName(QString::fromUtf8("treeViewBranch"));

        gridLayout->addWidget(treeViewBranch, 1, 0, 1, 1);


        gridLayout_3->addWidget(groupBoxBranchManager, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButtonOk = new QPushButton(BranchWidget);
        pushButtonOk->setObjectName(QString::fromUtf8("pushButtonOk"));

        horizontalLayout->addWidget(pushButtonOk);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButtonCancel = new QPushButton(BranchWidget);
        pushButtonCancel->setObjectName(QString::fromUtf8("pushButtonCancel"));

        horizontalLayout->addWidget(pushButtonCancel);


        gridLayout_3->addLayout(horizontalLayout, 2, 0, 1, 1);


        retranslateUi(BranchWidget);

        QMetaObject::connectSlotsByName(BranchWidget);
    } // setupUi

    void retranslateUi(QWidget *BranchWidget)
    {
        BranchWidget->setWindowTitle(QApplication::translate("BranchWidget", "Form", 0, QApplication::UnicodeUTF8));
        groupBoxCurrentBranch->setTitle(QApplication::translate("BranchWidget", "\320\242\320\265\320\272\321\203\321\211\320\270\320\271 \321\204\320\270\320\273\320\270\320\260\320\273:", 0, QApplication::UnicodeUTF8));
        groupBoxBranchManager->setTitle(QApplication::translate("BranchWidget", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265 \321\204\320\270\320\273\320\270\320\260\320\273\320\260\320\274\320\270", 0, QApplication::UnicodeUTF8));
        pushButtonAddBranch->setText(QApplication::translate("BranchWidget", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        pushButtonRemoveBranch->setText(QApplication::translate("BranchWidget", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        pushButtonOk->setText(QApplication::translate("BranchWidget", "\320\236\320\272", 0, QApplication::UnicodeUTF8));
        pushButtonCancel->setText(QApplication::translate("BranchWidget", "\320\236\321\202\320\274\320\265\320\275\320\260", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class BranchWidget: public Ui_BranchWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BRANCHWIDGET_H

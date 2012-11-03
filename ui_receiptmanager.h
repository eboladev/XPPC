/********************************************************************************
** Form generated from reading UI file 'receiptmanager.ui'
**
** Created: Sat Nov 3 13:39:14 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RECEIPTMANAGER_H
#define UI_RECEIPTMANAGER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ReceiptManager
{
public:
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_3;
    QLabel *labelFIO;
    QLineEdit *lineEditFIO;
    QLabel *labelPhone;
    QLineEdit *lineEditPhone;
    QLabel *labelDevice;
    QLineEdit *lineEditDevice;
    QLabel *labelSN;
    QLineEdit *lineEditSerial;
    QLabel *labelCondition;
    QLineEdit *lineEditQual;
    QGroupBox *groupBoxMalfunction;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *labelMalfunction;
    QPlainTextEdit *plainTextEditMalfunction;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonAddReceipt;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonClearFields;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButtonClose;

    void setupUi(QDialog *ReceiptManager)
    {
        if (ReceiptManager->objectName().isEmpty())
            ReceiptManager->setObjectName(QString::fromUtf8("ReceiptManager"));
        ReceiptManager->resize(503, 266);
        ReceiptManager->setMinimumSize(QSize(500, 260));
        gridLayout_2 = new QGridLayout(ReceiptManager);
        gridLayout_2->setSpacing(2);
        gridLayout_2->setContentsMargins(2, 2, 2, 2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        groupBox = new QGroupBox(ReceiptManager);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMaximumSize(QSize(230, 16777215));
        gridLayout_3 = new QGridLayout(groupBox);
        gridLayout_3->setSpacing(1);
        gridLayout_3->setContentsMargins(2, 2, 2, 2);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        labelFIO = new QLabel(groupBox);
        labelFIO->setObjectName(QString::fromUtf8("labelFIO"));

        gridLayout_3->addWidget(labelFIO, 0, 0, 1, 1);

        lineEditFIO = new QLineEdit(groupBox);
        lineEditFIO->setObjectName(QString::fromUtf8("lineEditFIO"));

        gridLayout_3->addWidget(lineEditFIO, 1, 0, 1, 1);

        labelPhone = new QLabel(groupBox);
        labelPhone->setObjectName(QString::fromUtf8("labelPhone"));

        gridLayout_3->addWidget(labelPhone, 2, 0, 1, 1);

        lineEditPhone = new QLineEdit(groupBox);
        lineEditPhone->setObjectName(QString::fromUtf8("lineEditPhone"));

        gridLayout_3->addWidget(lineEditPhone, 3, 0, 1, 1);

        labelDevice = new QLabel(groupBox);
        labelDevice->setObjectName(QString::fromUtf8("labelDevice"));

        gridLayout_3->addWidget(labelDevice, 4, 0, 1, 1);

        lineEditDevice = new QLineEdit(groupBox);
        lineEditDevice->setObjectName(QString::fromUtf8("lineEditDevice"));

        gridLayout_3->addWidget(lineEditDevice, 5, 0, 1, 1);

        labelSN = new QLabel(groupBox);
        labelSN->setObjectName(QString::fromUtf8("labelSN"));

        gridLayout_3->addWidget(labelSN, 6, 0, 1, 1);

        lineEditSerial = new QLineEdit(groupBox);
        lineEditSerial->setObjectName(QString::fromUtf8("lineEditSerial"));

        gridLayout_3->addWidget(lineEditSerial, 7, 0, 1, 1);

        labelCondition = new QLabel(groupBox);
        labelCondition->setObjectName(QString::fromUtf8("labelCondition"));

        gridLayout_3->addWidget(labelCondition, 8, 0, 1, 1);

        lineEditQual = new QLineEdit(groupBox);
        lineEditQual->setObjectName(QString::fromUtf8("lineEditQual"));

        gridLayout_3->addWidget(lineEditQual, 9, 0, 1, 1);


        horizontalLayout_2->addWidget(groupBox);

        groupBoxMalfunction = new QGroupBox(ReceiptManager);
        groupBoxMalfunction->setObjectName(QString::fromUtf8("groupBoxMalfunction"));
        gridLayout = new QGridLayout(groupBoxMalfunction);
        gridLayout->setSpacing(1);
        gridLayout->setContentsMargins(2, 2, 2, 2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelMalfunction = new QLabel(groupBoxMalfunction);
        labelMalfunction->setObjectName(QString::fromUtf8("labelMalfunction"));

        verticalLayout->addWidget(labelMalfunction);

        plainTextEditMalfunction = new QPlainTextEdit(groupBoxMalfunction);
        plainTextEditMalfunction->setObjectName(QString::fromUtf8("plainTextEditMalfunction"));

        verticalLayout->addWidget(plainTextEditMalfunction);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        horizontalLayout_2->addWidget(groupBoxMalfunction);


        gridLayout_2->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButtonAddReceipt = new QPushButton(ReceiptManager);
        pushButtonAddReceipt->setObjectName(QString::fromUtf8("pushButtonAddReceipt"));

        horizontalLayout->addWidget(pushButtonAddReceipt);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButtonClearFields = new QPushButton(ReceiptManager);
        pushButtonClearFields->setObjectName(QString::fromUtf8("pushButtonClearFields"));

        horizontalLayout->addWidget(pushButtonClearFields);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        pushButtonClose = new QPushButton(ReceiptManager);
        pushButtonClose->setObjectName(QString::fromUtf8("pushButtonClose"));

        horizontalLayout->addWidget(pushButtonClose);


        gridLayout_2->addLayout(horizontalLayout, 1, 0, 1, 1);

#ifndef QT_NO_SHORTCUT
        labelFIO->setBuddy(lineEditFIO);
        labelPhone->setBuddy(lineEditPhone);
        labelDevice->setBuddy(lineEditDevice);
        labelSN->setBuddy(lineEditSerial);
        labelCondition->setBuddy(lineEditQual);
        labelMalfunction->setBuddy(plainTextEditMalfunction);
#endif // QT_NO_SHORTCUT

        retranslateUi(ReceiptManager);

        QMetaObject::connectSlotsByName(ReceiptManager);
    } // setupUi

    void retranslateUi(QDialog *ReceiptManager)
    {
        ReceiptManager->setWindowTitle(QApplication::translate("ReceiptManager", "\320\224\320\276\320\261\320\260\320\262\320\273\320\265\320\275\320\270\320\265 \320\272\320\262\320\270\321\202\320\260\320\275\321\206\320\270\320\270", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString());
        labelFIO->setText(QApplication::translate("ReceiptManager", "\320\244\320\260\320\274\320\270\320\273\320\270\321\217, \320\230\320\274\321\217, \320\236\321\202\321\207\320\265\321\201\321\202\320\262\320\276:", 0, QApplication::UnicodeUTF8));
        labelPhone->setText(QApplication::translate("ReceiptManager", "\320\242\320\265\320\273\320\265\321\204\320\276\320\275:", 0, QApplication::UnicodeUTF8));
        labelDevice->setText(QApplication::translate("ReceiptManager", "\320\243\321\201\321\202\321\200\320\276\320\271\321\201\321\202\320\262\320\276:", 0, QApplication::UnicodeUTF8));
        labelSN->setText(QApplication::translate("ReceiptManager", "\320\241\320\265\321\200\320\270\320\271\320\275\321\213\320\271 \320\275\320\276\320\274\320\265\321\200:", 0, QApplication::UnicodeUTF8));
        labelCondition->setText(QApplication::translate("ReceiptManager", "\320\241\320\276\321\201\321\202\320\276\321\217\320\275\320\270\320\265:", 0, QApplication::UnicodeUTF8));
        groupBoxMalfunction->setTitle(QString());
        labelMalfunction->setText(QApplication::translate("ReceiptManager", "\320\227\320\260\321\217\320\262\320\273\320\265\320\275\320\275\320\260\321\217 \320\275\320\265\320\270\321\201\320\277\321\200\320\260\320\262\320\275\320\276\321\201\321\202\321\214", 0, QApplication::UnicodeUTF8));
        pushButtonAddReceipt->setText(QApplication::translate("ReceiptManager", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \320\272\320\262\320\270\321\202\320\260\320\275\321\206\320\270\321\216", 0, QApplication::UnicodeUTF8));
        pushButtonClearFields->setText(QApplication::translate("ReceiptManager", "\320\236\321\207\320\270\321\201\321\202\320\270\321\202\321\214 \320\277\320\276\320\273\321\217", 0, QApplication::UnicodeUTF8));
        pushButtonClose->setText(QApplication::translate("ReceiptManager", "\320\222\321\213\320\271\321\202\320\270", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ReceiptManager: public Ui_ReceiptManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RECEIPTMANAGER_H

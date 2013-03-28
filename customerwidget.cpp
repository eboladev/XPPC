#include "customerwidget.h"
#include "setupmanager.h"

#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCompleter>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QTreeView>
#include <QHeaderView>

CustomerWidget::CustomerWidget(QWidget *parent) :
    QWidget(parent)
{    
    currentClientId = -1;
    nameLabel = new QLabel(trUtf8("ФИО:"), this);
    phoneLabel = new QLabel(trUtf8("Телефон:"), this);
    nameEdit = new QLineEdit(this);
    phoneEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText(trUtf8("ФИО:"));
    phoneEdit->setPlaceholderText(trUtf8("Телефон:"));
    QVBoxLayout* vl = new QVBoxLayout();
    QHBoxLayout* horNameLayout = new QHBoxLayout();
    horNameLayout->addWidget(nameLabel);
    horNameLayout->addWidget(nameEdit);
    QHBoxLayout* horPhoneLayout= new QHBoxLayout();
    horPhoneLayout->addWidget(phoneLabel);
    horPhoneLayout->addWidget(phoneEdit);
    vl->addLayout(horNameLayout);
    vl->addLayout(horPhoneLayout);
    setLayout(vl);

    if (SetupManager::instance()->getClientNameCompleterEnabled())
    {
        model = new QStandardItemModel(this);
        proxy = new QSortFilterProxyModel(this);
        proxy->setSourceModel(model);
        proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);        
        connect(nameEdit, SIGNAL(textChanged(QString)), proxy, SLOT(setFilterFixedString(QString)));        
        QTreeView* tw = new QTreeView(this);
        tw->setModel(proxy);
        tw->setHeaderHidden(true);
        tw->setIndentation(-1);
        tw->resizeColumnToContents(0);
        QHeaderView* hw = tw->header();
        hw->setStretchLastSection(false);
        hw->setCascadingSectionResizes(true);
        hw->setDefaultSectionSize(150);
        tw->setHeader(hw);

        QCompleter *completer = new QCompleter(proxy, this);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        completer->setPopup(tw);
        completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
        nameEdit->setCompleter(completer);

        connect(completer, SIGNAL(activated(QModelIndex)), SLOT(onCompleteClientData(QModelIndex)));

        QSqlQuery q;
        if (!SetupManager::instance()->getSqlQueryForDB(q))
            return;
        q.exec("select id, name, phone from client ");
        while(q.next())
        {
            QStandardItem* item = new QStandardItem(q.value(1).toString());
            item->setData(q.value(0),IdRole);
            item->setData(q.value(2).toString(),PhoneRole);
            item->setToolTip(trUtf8("Телефон: %0").arg(q.value(2).toString()));            
            QStandardItem* item2 = new QStandardItem(q.value(2).toString());
            model->appendRow(QList<QStandardItem*>() << item << item2);
            //model->appendRow(item);
        }
    }
}

CustomerWidget::~CustomerWidget()
{
}

void CustomerWidget::setName(const QString &name)
{    
    nameEdit->setText(name);
}

void CustomerWidget::setPhone(const QString &phone)
{
    phoneEdit->setText(phone);
}

QString CustomerWidget::getName() const
{
    return nameEdit->text();
}

QString CustomerWidget::getPhone() const
{
    return phoneEdit->text();
}

void CustomerWidget::setCurrentClientId(const int &id)
{
    currentClientId = id;
}

QCompleter *CustomerWidget::getCompleter()
{
    return nameEdit->completer();
}

void CustomerWidget::onCompleteClientData(QModelIndex index)
{
    setPhone(getCompleter()->completionModel()->data(getCompleter()->completionModel()->index(index.row(),0), PhoneRole).toString());
    setCurrentClientId(getCompleter()->completionModel()->data(getCompleter()->completionModel()->index(index.row(),0), IdRole).toInt());
}

int CustomerWidget::getCurrentClientId() const
{
    return currentClientId;
}

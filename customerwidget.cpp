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

CustomerWidget::CustomerWidget(QWidget *parent) :
    QWidget(parent)
{    
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

        QCompleter *completer = new QCompleter(proxy, this);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
        nameEdit->setCompleter(completer);

        QSqlQuery q;
        if (!SetupManager::instance()->getSqlQueryForDB(q))
            return;
        q.exec("select id, name, phone from client ");//where name LIKE '%"+FIO+"%'
        while(q.next())
        {
            QStandardItem* item = new QStandardItem(q.value(1).toString());
            item->setData(q.value(0));
            item->setToolTip(trUtf8("Телефон: %0").arg(q.value(2).toString()));
            model->appendRow(item);
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

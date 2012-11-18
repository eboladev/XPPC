#include "serialportcomboboxdelegate.h"
#include "sendsms.h"
#include <QComboBox>
#include <QLineEdit>

SerialPortComboBoxDelegate::SerialPortComboBoxDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget *SerialPortComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *cb = new QComboBox(parent);
    cb->setEditable(true);
    QStringList avialablePorts = SendSMS::getAvailablePorts();
    cb->addItems(avialablePorts);
    return cb;
}


void SerialPortComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    QRect rect = option.rect;
    QSize sizeHint = editor->sizeHint();
    if (rect.width()<sizeHint.width()) rect.setWidth(sizeHint.width());
    if (rect.height()<sizeHint.height()) rect.setHeight(sizeHint.height());
    editor->setGeometry(rect);
}


void SerialPortComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *cb = qobject_cast<QComboBox*>(editor);
    if (!cb)
        return;
    model->setData(index, cb->currentText());
}

void SerialPortComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *cb = qobject_cast<QComboBox*>(editor);
    if (!cb)
        return;
    cb->lineEdit()->setText(index.data().toString());
}

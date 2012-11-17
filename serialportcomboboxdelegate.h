#ifndef SERIALPORTCOMBOBOXDELEGATE_H
#define SERIALPORTCOMBOBOXDELEGATE_H

#include <QStyledItemDelegate>
#include <QWidget>

class SerialPortComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit SerialPortComboBoxDelegate(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    
signals:
    
public slots:
    
};

#endif // SERIALPORTCOMBOBOXDELEGATE_H

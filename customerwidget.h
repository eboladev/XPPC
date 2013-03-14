#ifndef CUSTOMERWIDGET_H
#define CUSTOMERWIDGET_H

#include <QWidget>
#include <QCompleter>

class QLabel;
class QLineEdit;
class QStandardItemModel;
class QSortFilterProxyModel;

class CompleterWithUserData : public QCompleter
{
    Q_OBJECT
public:
    explicit CompleterWithUserData( QAbstractItemModel * model, QObject * parent = 0 );

    static const int CompleteRole;

    QString pathFromIndex(const QModelIndex &index) const;
};

class CustomerWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit CustomerWidget(QWidget *parent = 0);
    ~CustomerWidget();
    void setName(const QString& name);
    void setPhone(const QString& phone);
    QString getName() const;
    QString getPhone() const;
    QVariant getCustomerId() const;

private:
    QLabel* nameLabel;
    QLineEdit* nameEdit;
    QLabel* phoneLabel;
    QLineEdit* phoneEdit;
    QStandardItemModel* model;
    QSortFilterProxyModel* proxy;
};

#endif // CUSTOMERWIDGET_H

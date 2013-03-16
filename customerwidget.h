#ifndef CUSTOMERWIDGET_H
#define CUSTOMERWIDGET_H

#include <QWidget>
#include <QModelIndex>

class QLabel;
class QLineEdit;
class QStandardItemModel;
class QSortFilterProxyModel;
class QCompleter;

enum CompleterRoles
{
    IdRole = Qt::UserRole + 1,
    PhoneRole = Qt::UserRole + 2
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
    void setCurrentClientId(const int& id);
    int getCurrentClientId() const;

private:
    QCompleter* getCompleter();

private slots:    
    void onCompleteClientData(QModelIndex);

private:
    QLabel* nameLabel;
    QLineEdit* nameEdit;
    QLabel* phoneLabel;
    QLineEdit* phoneEdit;
    QStandardItemModel* model;
    QSortFilterProxyModel* proxy;
    int currentClientId;
};

#endif // CUSTOMERWIDGET_H

#ifndef USERINFOWIDGET_H
#define USERINFOWIDGET_H

#include <QWidget>
#include <QVariant>

namespace Ui {
class UserInfoWidget;
}

class QStandardItemModel;

class UserInfoWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit UserInfoWidget(QWidget *parent = 0);
    ~UserInfoWidget();
    void setUserId(const QVariant& id);
    void setUserName(const QString& name);
    void setUserPhone(const QString& phone);
    void setUserLogin(const QString& login);
    void setUserRate(const int& rate);
    void setUserPercent(const int& percent);
    void setUserSalePercent(const int& sale_percent);
    void setUserSalaryPerDay(const int& spDay);
    void setCurrentUserGroup(const QVariant& group_id);
    void setGroupsEditable(const bool& isEditable);

    QVariant getUserId() const;
    QString getUserName() const;
    QString getUserPhone() const;
    QString getUserLogin() const;
    int getUserRate() const;
    int getUserPercent() const;
    int getUserSalePercent() const;
    int getUserSalaryPerDay() const;
    QVariant getCurrentGroupId() const;

    void setItemModelForGroupsComboBox(QStandardItemModel* model);

private slots:    
    void onChangeLoginpass();

signals:
    void userNameChanged(QString userName);
    void changesSaved();

private:
    Ui::UserInfoWidget *ui;
    QVariant userId;
};

#endif // USERINFOWIDGET_H

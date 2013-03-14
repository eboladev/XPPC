#ifndef CUSTOMERWIDGET_H
#define CUSTOMERWIDGET_H

#include <QWidget>

class QLabel;
class QLineEdit;

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
    
private:
    QLabel* nameLabel;
    QLabel* phoneLabel;
    QLineEdit* nameEdit;
    QLineEdit* phoneEdit;

};

#endif // CUSTOMERWIDGET_H

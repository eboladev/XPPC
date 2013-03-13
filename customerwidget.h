#ifndef CUSTOMERWIDGET_H
#define CUSTOMERWIDGET_H

#include <QWidget>

namespace Ui {
class CustomerWidget;
}

class CustomerWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit CustomerWidget(QWidget *parent = 0);
    ~CustomerWidget();
    void setName( QString name);
    void setPhone( QString phone);
    QString getName() const;
    QString getPhone() const;
    
private:
    Ui::CustomerWidget *ui;
};

#endif // CUSTOMERWIDGET_H

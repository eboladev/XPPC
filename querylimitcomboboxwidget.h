#ifndef QUERYLIMITCOMBOBOXWIDGET_H
#define QUERYLIMITCOMBOBOXWIDGET_H

#include <QWidget>

namespace Ui {
class QueryLimitComboBoxWidget;
}

class QueryLimitComboBoxWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit QueryLimitComboBoxWidget(QWidget *parent = 0);
    ~QueryLimitComboBoxWidget();
    int getLimit();

signals:
    void limitChanged(int);

private:
    Ui::QueryLimitComboBoxWidget *ui;
};

#endif // QUERYLIMITCOMBOBOXWIDGET_H

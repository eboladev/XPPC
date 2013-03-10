#ifndef USERSTATISTICWIDGET_H
#define USERSTATISTICWIDGET_H

#include <QWidget>

namespace Ui {
class UserStatisticWidget;
}

class UserStatisticWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit UserStatisticWidget(QWidget *parent = 0);
    ~UserStatisticWidget();
    
private:
    Ui::UserStatisticWidget *ui;
};

#endif // USERSTATISTICWIDGET_H

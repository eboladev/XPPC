#ifndef PERMISSIONSWIDGET_H
#define PERMISSIONSWIDGET_H

#include <QWidget>

namespace Ui {
class PermissionsWidget;
}

class PermissionsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PermissionsWidget(QWidget *parent = 0);
    ~PermissionsWidget();
    
private:
    Ui::PermissionsWidget *ui;
};

#endif // PERMISSIONSWIDGET_H

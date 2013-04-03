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
    int getCurrentPermissions() const;

public slots:
    void onSetPermissions(const int& value);

private slots:
    void onSaveChangesClicked();

signals:
    void setPermissions(const int& value);

private:
    Ui::PermissionsWidget *ui;
};

#endif // PERMISSIONSWIDGET_H

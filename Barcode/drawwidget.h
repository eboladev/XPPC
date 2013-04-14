#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QWidget>

namespace Ui {
class DrawWidget;
}

class DrawWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit DrawWidget(QWidget *parent = 0);
    ~DrawWidget();
    
    void draw(QPainter *painter);

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::DrawWidget *ui;
};

#endif // DRAWWIDGET_H

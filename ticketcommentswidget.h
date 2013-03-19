#ifndef TICKETCOMMENTSWIDGET_H
#define TICKETCOMMENTSWIDGET_H

#include <QWidget>

namespace Ui {
class TicketCommentsWidget;
}

class TicketCommentsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit TicketCommentsWidget(QWidget *parent = 0);
    ~TicketCommentsWidget();
    
private:
    Ui::TicketCommentsWidget *ui;
};

#endif // TICKETCOMMENTSWIDGET_H

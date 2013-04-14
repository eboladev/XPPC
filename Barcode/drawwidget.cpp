#include "drawwidget.h"
#include "ui_drawwidget.h"

#include <QPainter>
#include "ean13.h"


DrawWidget::DrawWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DrawWidget)
{
    ui->setupUi(this);
}

DrawWidget::~DrawWidget()
{
    delete ui;
}

void DrawWidget::draw(QPainter *painter)
{
    painter->fillRect(painter->window(), Qt::lightGray);

    EAN13 code("4607036044365");

    painter->drawText(50,40,"Arbitrary size (400x200pix)");

    code.draw(QRectF(50, 50, 400, 200), *painter);

    qreal scale = painter->device()->physicalDpiX() / 25.4;	// dots per mm

    painter->drawText(50,290,"Nominal size (37.29x25.93mm)");

    code.draw(QRectF(50, 300, 37.29 * scale, 25.93 * scale), *painter);
}

void DrawWidget::paintEvent(QPaintEvent *event)
{
    event = event;										// evade compiler warning
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setViewport(0,0, width(), width());

    draw(&painter);
}

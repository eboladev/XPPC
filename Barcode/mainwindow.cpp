#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "code128generator.h"

#include <QPaintEvent>
#include <QPainter>
#include <QFontDatabase>
#include <QDebug>
#include <QChar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*QFontDatabase fontDatabase;
    qDebug() << fontDatabase.addApplicationFont("://code128.ttf");
    qDebug() << fontDatabase.applicationFontFamilies(0);
    qDebug() << QString(" ").toAscii() << QChar(' ').unicode() << QChar(32);*/
    c128g = new Code128Generator(1,200,"0105480100710_",this,static_cast<Code128Generator::Code128Type>(1));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::black);
    painter.setBrush(brush);
    qreal scale = painter.device()->physicalDpiX() / 25.4;	// dots per mm
    c128g->drawBarcode(QRectF(50, 300, 37.29 * scale*8, 25.93 * scale),painter);
}

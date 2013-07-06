#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Code128Generator;
class QPaintEvent;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *);
    
private:
    Ui::MainWindow *ui;
    Code128Generator* c128g;
};

#endif // MAINWINDOW_H

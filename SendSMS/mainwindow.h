#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainSMSHandler;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onAnswer(const QString& answer);

private slots:
    void onSend();
    void onStatus();
    void onBalance();
    void onPrice();
    void onTestModeSwitch();

private:
    Ui::MainWindow *ui;
    MainSMSHandler* msmsh;
};

#endif // MAINWINDOW_H

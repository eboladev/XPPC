#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mainsmshandler.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    msmsh = new MainSMSHandler("gradosh", "gradosh", "",this);
    msmsh->send(trUtf8("атата атата атата"), QStringList() << "" << "");
    msmsh->status(QStringList() << "1" << "2" << "3" << "4");
    msmsh->price("asddd", QStringList() << "" << "");
    msmsh->balance();
}

MainWindow::~MainWindow()
{
    delete ui;
}

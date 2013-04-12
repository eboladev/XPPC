#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mainsmshandler.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    msmsh = new MainSMSHandler(this);
    msmsh->setLogin("");
    msmsh->setPassword("");
    msmsh->setSenderName("");
    //msmsh->setTestMode(true);
    connect(ui->pushButtonSend, SIGNAL(clicked()), this, SLOT(onSend()));
    connect(ui->pushButtonBalance, SIGNAL(clicked()), this, SLOT(onBalance()));
    connect(ui->pushButtonStatus, SIGNAL(clicked()), this, SLOT(onStatus()));
    connect(ui->pushButtonPrice, SIGNAL(clicked()), this, SLOT(onPrice()));
    connect(msmsh, SIGNAL(answer(QString)), this, SLOT(onAnswer(QString)));
    connect(ui->checkBoxTest, SIGNAL(clicked()), this, SLOT(onTestModeSwitch()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAnswer(const QString &answer)
{
    ui->plainTextEditAnswer->appendPlainText(answer);
    ui->plainTextEditAnswer->appendPlainText("\n");
}

void MainWindow::onSend()
{
    msmsh->send(ui->plainTextEditMessage->toPlainText(),ui->lineEditRecipients->text().split(";"));
}

void MainWindow::onStatus()
{
    msmsh->status(ui->lineEditStatus->text().split(";"));//<< "89657376069"
}

void MainWindow::onBalance()
{
    msmsh->balance();
}

void MainWindow::onPrice()
{
    msmsh->price(ui->plainTextEditMessage->toPlainText(),ui->lineEditRecipients->text().split(";"));
}

void MainWindow::onTestModeSwitch()
{
    msmsh->setTestMode(ui->checkBoxTest->checkState() == Qt::Checked ? true : false);
}

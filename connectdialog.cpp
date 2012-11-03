#include "connectdialog.h"
#include "ui_connectdialog.h"
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QSettings>

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);
    connect(ui->pushButtonConnect, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->pushButtonClose,SIGNAL(clicked()),this, SLOT(reject()));
    connect(ui->pushButtonConnect,SIGNAL(clicked()),this,SLOT(saveSettings()));
    loadSettings();
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

void ConnectDialog::loadSettings()
{
    QSettings settings;
    //settings.sync();
    if (settings.allKeys().count() >0)
    {
    ui->lineEditHost->setText(settings.value("db/HostName").toString());
    ui->lineEditName->setText(settings.value("db/UserName").toString());
    QByteArray ba = settings.value("db/Password").toByteArray();
    encryptDecrypt(ba);
    ui->lineEditPassword->setText(QString::fromUtf8(ba.data(), ba.count()));
    ui->lineEditPort->setText(settings.value("db/Port").toString());
    ui->lineEditDBName->setText(settings.value("db/DatabaseName").toString());
    }
    else
    {
        ui->lineEditHost->setText("195.46.162.200");
        ui->lineEditName->setText("admin");
        ui->lineEditPassword->setText("");
        ui->lineEditPort->setText("3306");
        ui->lineEditDBName->setText("Service_test");
    }
}

void ConnectDialog::saveSettings()
{
    QSettings settings;
    settings.beginGroup("db");
    settings.setValue("DatabaseName",ui->lineEditDBName->text());
    settings.setValue("HostName",ui->lineEditHost->text());
    settings.setValue("Port",ui->lineEditPort->text());
    settings.setValue("UserName",ui->lineEditName->text());
    qDebug() << "pass entered" << ui->lineEditPassword->text();
    QByteArray ba = ui->lineEditPassword->text().toUtf8();
    encryptDecrypt(ba);
    settings.setValue("Password",ba);
    qDebug() << "password saved " << ba;
    settings.endGroup();
}

void ConnectDialog::encryptDecrypt(QByteArray &ba)
{
    char c = 55;
    for (int i=0; i<ba.count();++i){
        ba[i] = ba.at(i) ^ c;
        c += 34;
    }
}

void ConnectDialog::on_pushButtonTestConnection_clicked()
{
    QSqlDatabase mySQLDatabase;
    mySQLDatabase = QSqlDatabase::database(QLatin1String("XP"));
    if (!mySQLDatabase.isOpen())
    {
    mySQLDatabase = QSqlDatabase::addDatabase("QMYSQL", QLatin1String("XP"));
    mySQLDatabase.setDatabaseName(QString(ui->lineEditDBName->text()).trimmed());
    mySQLDatabase.setHostName(QString(ui->lineEditHost->text()).trimmed());
    mySQLDatabase.setUserName(QString(ui->lineEditName->text()).trimmed());
    mySQLDatabase.setPassword(QString(ui->lineEditPassword->text()).trimmed());
    mySQLDatabase.setPort(ui->lineEditPort->text().toInt());
    if  (mySQLDatabase.open())
        {
        QDialog dialog;
        QHBoxLayout *lay = new QHBoxLayout(&dialog);
        dialog.setLayout(lay);
        QLabel *su = new QLabel("Connection succesfull");
        lay->addWidget(su);
        dialog.setWindowTitle("Connection result");
        dialog.exec();
        }
            else
    {
        QDialog dialog;
        QHBoxLayout *lay = new QHBoxLayout(&dialog);
        dialog.setLayout(lay);
        QLabel *su = new QLabel("Connection failed");
        lay->addWidget(su);        
        dialog.setWindowTitle("Connection result");
        dialog.exec();
    }
    }
        else
    {
        QDialog dialog;
        QHBoxLayout *lay = new QHBoxLayout(&dialog);
        dialog.setLayout(lay);
        QLabel *su = new QLabel("Connection succesfull");
        lay->addWidget(su);
        dialog.setWindowTitle("Connection result");
        dialog.exec();
    }
}

#include "connectdialog.h"
#include "ui_connectdialog.h"

#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QSettings>

const QString CONNECTIONNAME = "XP";

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
        ui->lineEditName->setText("sysdba");
        ui->lineEditPassword->setText("");
        ui->lineEditPort->setText("3050");
        ui->lineEditDBName->setText("hpnew");
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
    QByteArray ba = ui->lineEditPassword->text().toUtf8();
    encryptDecrypt(ba);
    settings.setValue("Password",ba);
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
    QDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    QHBoxLayout *lay = new QHBoxLayout(&dialog);
    QLabel *su = new QLabel();
    dialog.setLayout(lay);
    lay->addWidget(su);

    QSqlDatabase fireBirdSQLDatabase =  QSqlDatabase::database(CONNECTIONNAME.toLatin1(), false);

    if (!fireBirdSQLDatabase.isOpen())
    {
        fireBirdSQLDatabase = QSqlDatabase::addDatabase("QFIREBIRD", CONNECTIONNAME.toLatin1());//QMYSQL
        fireBirdSQLDatabase.setDatabaseName(ui->lineEditDBName->text().trimmed()); //getFBFileLocation()
        fireBirdSQLDatabase.setHostName(ui->lineEditHost->text().trimmed());
        fireBirdSQLDatabase.setUserName(ui->lineEditName->text().trimmed());
        fireBirdSQLDatabase.setPassword(ui->lineEditPassword->text().trimmed());
        //fireBirdSQLDatabase.setPort(ui->lineEditPort->text().toInt());
        fireBirdSQLDatabase.setConnectOptions("CHARSET=UTF-8");
        if  (fireBirdSQLDatabase.open())
        {
            su->setText("Connection succesfull");
            dialog.setWindowTitle("Connection result");
        }
        else
        {
            su->setText("Connection failed");
            dialog.setWindowTitle("Connection result");
        }
    }
    else
    {
        su->setText("Connection succesfull and already opened");
        dialog.setWindowTitle("Connection result");
    }
    dialog.exec();
    fireBirdSQLDatabase.close();
    QSqlDatabase::removeDatabase(CONNECTIONNAME);
    /*mysql*/
    //    QSqlDatabase mySQLDatabase;
    //    mySQLDatabase = QSqlDatabase::database(QLatin1String("XP"));
    //    if (!mySQLDatabase.isOpen())
    //    {
    //        mySQLDatabase = QSqlDatabase::addDatabase("QMYSQL", QLatin1String("XP"));
    //        mySQLDatabase.setDatabaseName(QString(ui->lineEditDBName->text()).trimmed());
    //        mySQLDatabase.setHostName(QString(ui->lineEditHost->text()).trimmed());
    //        mySQLDatabase.setUserName(QString(ui->lineEditName->text()).trimmed());
    //        mySQLDatabase.setPassword(QString(ui->lineEditPassword->text()).trimmed());
    //        mySQLDatabase.setPort(ui->lineEditPort->text().toInt());
    //        if  (mySQLDatabase.open())
    //        {
    //            su->setText("Connection succesfull");
    //            dialog.setWindowTitle("Connection result");
    //        }
    //        else
    //        {
    //            su->setText("Connection failed");
    //            dialog.setWindowTitle("Connection result");
    //        }
    //    }
    //    else
    //    {
    //        su->setText("Connection succesfull and already opened");
    //        dialog.setWindowTitle("Connection result");
    //    }
    //    dialog.exec();
}

void ConnectDialog::on_pushButtonConnect_clicked()
{
    if (QSqlDatabase::database(QLatin1String("XP")).isOpen())
        QSqlDatabase::database(QLatin1String("XP")).close();
    QSqlDatabase::removeDatabase(QLatin1String("XP"));
}

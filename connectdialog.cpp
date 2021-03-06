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
        ui->lineEditName->setText("");
        ui->lineEditPassword->setText("");
        ui->lineEditPort->setText("5432");
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
    dialog.setWindowTitle("Connection result");
    QSqlDatabase fireBirdSQLDatabase =  QSqlDatabase::database(CONNECTIONNAME.toLatin1(), false);

    if (fireBirdSQLDatabase.isOpen())
    {
        fireBirdSQLDatabase.close();
        QSqlDatabase::removeDatabase(CONNECTIONNAME);
        qDebug() << QSqlDatabase::connectionNames() << fireBirdSQLDatabase.isOpen() << fireBirdSQLDatabase.isValid();
    }
        fireBirdSQLDatabase = QSqlDatabase::addDatabase("QPSQL", CONNECTIONNAME.toLatin1());
        fireBirdSQLDatabase.setDatabaseName(ui->lineEditDBName->text().trimmed());
        fireBirdSQLDatabase.setHostName(ui->lineEditHost->text().trimmed());
        fireBirdSQLDatabase.setUserName(ui->lineEditName->text().trimmed());
        fireBirdSQLDatabase.setPassword(ui->lineEditPassword->text().trimmed());
        fireBirdSQLDatabase.setPort(ui->lineEditPort->text().toInt());
        if  (fireBirdSQLDatabase.open())        
            su->setText("Connection succesfull");                    
        else        
            su->setText("Connection failed");
        fireBirdSQLDatabase.close();
        QSqlDatabase::removeDatabase(CONNECTIONNAME);
        qDebug() << QSqlDatabase::connectionNames();
    /*}
    else    
        su->setText("Connection succesfull and already opened");*/
    dialog.exec();
}

void ConnectDialog::on_pushButtonConnect_clicked()
{
    if (QSqlDatabase::database(QLatin1String("XP")).isOpen())
        QSqlDatabase::database(QLatin1String("XP")).close();
    QSqlDatabase::removeDatabase(QLatin1String("XP"));
}

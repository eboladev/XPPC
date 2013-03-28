#include "changeuserdialog.h"
#include "ui_changeuserdialog.h"
#include "setupmanager.h"
ChangeUserDialog::ChangeUserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeUserDialog)
{
    ui->setupUi(this);
    connect(ui->pushButtonOk, SIGNAL(clicked()), SLOT(onAccept()));
    connect(ui->pushButtonCancel, SIGNAL(clicked()), SLOT(reject()));
    connect(ui->pushButtonLogout, SIGNAL(clicked()), SLOT(onLogout()));
#ifdef RELEASE
    ui->lineEditLogin->clear();
    ui->lineEditPassword->clear();
#endif
    setProperty("login",ui->lineEditLogin->text());
    setProperty("password",ui->lineEditPassword->text());
}

ChangeUserDialog::~ChangeUserDialog()
{    
    delete ui;
}

QString ChangeUserDialog::getUser()
{
    return ui->lineEditLogin->text().trimmed();
}

QString ChangeUserDialog::getPassword()
{
    return ui->lineEditPassword->text().trimmed();
}

void ChangeUserDialog::onSuccesfullLogin()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void ChangeUserDialog::onAccept()
{
    setProperty("login",ui->lineEditLogin->text());
    setProperty("password",ui->lineEditPassword->text());
    accept();
}

void ChangeUserDialog::onLogout()
{
    ui->stackedWidget->setCurrentIndex(0);
    SetupManager::instance()->setCurrentUser("");
    SetupManager::instance()->setPermissons(0);
    ui->lineEditLogin->setText("");
    ui->lineEditPassword->setText("");
    emit changePermissions();
}

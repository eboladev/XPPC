#include "changeuserdialog.h"
#include "ui_changeuserdialog.h"
#include <QDebug>
ChangeUserDialog::ChangeUserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeUserDialog)
{
    ui->setupUi(this);
    connect(ui->pushButtonOk, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->pushButtonCancel, SIGNAL(clicked()), SLOT(reject()));
    connect(ui->pushButtonLogout, SIGNAL(clicked()), SLOT(onLogout()));    
#ifdef RELEASE
    ui->lineEditLogin->clear();
    ui->lineEditPassword->clear();
#endif
}

ChangeUserDialog::~ChangeUserDialog()
{    
    delete ui;
}

QString ChangeUserDialog::getUser()
{
    return ui->lineEditLogin->text().trimmed();
}

void ChangeUserDialog::setUser(const QString &userLogin)
{
    ui->lineEditLogin->setText(userLogin);
}

QString ChangeUserDialog::getPassword()
{
    return ui->lineEditPassword->text().trimmed();
}

void ChangeUserDialog::onSuccesfullLogin()
{
    ui->stackedWidget->setCurrentIndex(1);    
}

void ChangeUserDialog::onLogout()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->lineEditLogin->setFocus();  
    ui->lineEditPassword->setText("");
    emit userLogOut();
}

bool ChangeUserDialog::exec()
{
    ui->lineEditLogin->setFocus();
    return QDialog::exec();
}

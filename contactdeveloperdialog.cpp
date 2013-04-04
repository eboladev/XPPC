#include "contactdeveloperdialog.h"
#include "ui_contactdeveloperdialog.h"

#include "usersandpermissionsmanager.h"
#include "smtpsettingsdialog.h"
#include "setupmanager.h"

#include <QDebug>
#include <QMessageBox>
#include "SmtpClient/src/smtpclient.h"
#include "SmtpClient/src/mimehtml.h"

struct EMailToSend
{
    // Тело письма
    QString mailBody;

    // тема письма
    QString mailSubject;

    // список вложений
    QList<MimePart*> attachementsList;

    // список получателей
    QList<EmailAddress*> recipientsList;

    // идентификатор
    int id;
};

ContactDeveloperDialog::ContactDeveloperDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContactDeveloperDialog)
{
    ui->setupUi(this);
    ui->pushButtonSettings->setEnabled(accessManager->isCanEditPermissions());
    connect(ui->pushButtonCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui->pushButtonSettings, SIGNAL(clicked()), this, SLOT(onSettingsClicked()));
    connect(ui->pushButtonSend, SIGNAL(clicked()), this, SLOT(onAccept()));
}

ContactDeveloperDialog::~ContactDeveloperDialog()
{
    delete ui;
}

void ContactDeveloperDialog::onSettingsClicked()
{
    QSqlQuery q;
    if (!setupManager->getSqlQueryForDB(q))
        return;    
    if (!q.exec("select smtpaddress, smtpport, smtplogin, smtppassword, smtpsenderaddress, smtpsendername, smtpmailtoaddress from developer_contact_settings"))
        qDebug() << q.lastError() << q.lastQuery();
    SmtpSettingsDialog ssd;
    bool settingsIsEmpty = false;
    if (q.next())
    {

        ssd.setSMTPAddress(q.value(0).toString());
        ssd.setSMTPPort(q.value(1).toInt());
        ssd.setSMTPLogin(q.value(2).toString());
        QByteArray temp = q.value(3).toByteArray();
        setupManager->encryptDecrypt(temp);
        ssd.setSMTPPass(QString::fromUtf8(temp.data(), temp.count()));
        ssd.setSMTPSenderAddress(q.value(4).toString());
        ssd.setSMTPSenderName(q.value(5).toString());
        ssd.setMailToAddress(q.value(6).toString());
    }
    else
        settingsIsEmpty = true;

    if (ssd.exec())
    {
        if (settingsIsEmpty)
            q.prepare("INSERT INTO public.developer_contact_settings "
                      "(smtpaddress, smtpport, smtplogin, smtppassword, smtpsenderaddress, smtpsendername, smtpmailtoaddress) "
                  "VALUES "
                      "(?, ?, ?, ?, ?, ?, ?)");
        else
            q.prepare("update developer_contact_settings set smtpaddress = ?, smtpport = ?, smtplogin = ?, "
                      "smtppassword = ?, smtpsenderaddress = ?, smtpsendername = ?, smtpmailtoaddress = ?");
        q.addBindValue(ssd.getSMTPAddress());
        q.addBindValue(ssd.getSMTPPort());
        q.addBindValue(ssd.getSMTPLogin());
        QByteArray ba = ssd.getSMTPPass().toUtf8();
        SetupManager::encryptDecrypt(ba);
        q.addBindValue(ba);
        q.addBindValue(ssd.getSMTPSenderAddress());
        q.addBindValue(ssd.getSMTPSenderName());
        q.addBindValue(ssd.getMailToAddress());
        if (!q.exec())
            qDebug() << q.lastError() << q.lastQuery();
    }

}

void ContactDeveloperDialog::onAccept()
{
    if (ui->lineEditEmailTheme->text().isEmpty() ||
            ui->textEditEmailBody->toPlainText().isEmpty())
        return;

    QSqlQuery q;
    if (!setupManager->getSqlQueryForDB(q))
        return;
    if (!q.exec("select smtpaddress, smtpport, smtplogin, smtppassword, smtpsenderaddress, smtpsendername, smtpmailtoaddress from developer_contact_settings"))
        qDebug() << q.lastError() << q.lastQuery();

    if (!q.next())
        QMessageBox::critical(0, QObject::trUtf8("Ошибка"),
                              QObject::trUtf8("Отправка письма невозможна, отсутствуют настройки SMTP"));

    QString address, login, pass, senderAddress, senderName, mailToAddress;
    int port;

    address = q.value(0).toString();
    login = q.value(2).toString();
    QByteArray temp = q.value(3).toByteArray();
    setupManager->encryptDecrypt(temp);
    pass = QString::fromUtf8(temp.data(), temp.count());
    qDebug() << Q_FUNC_INFO << pass;
    senderAddress = q.value(4).toString();
    senderName = q.value(5).toString();
    mailToAddress = q.value(6).toString();
    port = q.value(1).toInt();

    EMailToSend currentEMail;
    QString mailSubject = QString("%0 : %1")
            .arg(accessManager->getCurrentUserDisplayName())
            .arg(ui->lineEditEmailTheme->text());
    currentEMail.mailSubject.append(mailSubject);
    currentEMail.mailBody.append(ui->textEditEmailBody->toHtml());
    currentEMail.recipientsList.append(new EmailAddress(mailToAddress));

    // создаём объект smtp клиента
    SmtpClient smtp(address, port, SmtpClient::TcpConnection);

    // имя пользователя и пароль
    smtp.setUser(login);
    smtp.setPassword(pass);

    // создаём сообщение
    MimeMessage message;
    EmailAddress ema(senderAddress, senderName);

    message.setSender(&ema);
    message.setSubject(currentEMail.mailSubject);

    // добавляем получателей
    foreach (EmailAddress *ea, currentEMail.recipientsList)
        message.addRecipient(ea);

    // текст нашего письма
    MimeHtml text;
    text.setHtml(currentEMail.mailBody);
    message.addPart(&text);
/*
    // добавляем вложения
    foreach(MimePart *mime, currentEMail.attachementsList)
    {
        message.addPart(mime);
        qDebug() << "mime^" << mime->getContentName() << mime->getContent().size();
    }*/

    // теперь мы можем послать нашу почту

    smtp.setEmailId(currentEMail.id);

    if (smtp.connectToHost())
    {
        if (smtp.login())
        {
            if (smtp.sendMail(message)){
                QMessageBox::information(0, QObject::trUtf8("Успех! :)"),
                                      QObject::trUtf8("Сообщение отправлено"));
                qDebug() << "email has been sent";
            }
        }
        else
        {
            QMessageBox::critical(0, QObject::trUtf8("Ошибка"),
                                  QObject::trUtf8("Не удалось зайти под учетной записью %0, проверьте логин\\пароль").arg(login));
            qDebug() << Q_FUNC_INFO << "failed to login";
            return;
        }

        smtp.quit();
    }
    else
    {
        QMessageBox::critical(0, QObject::trUtf8("Ошибка"),
                              QObject::trUtf8("Не удалось подключится к серверу %0, проверьте настройки").arg(address));
        qDebug() << Q_FUNC_INFO << "failed to connect to host";
        return;
    }

    foreach (EmailAddress *ea, currentEMail.recipientsList)
        message.deleteRecipient(ea);
    /*foreach(MimePart *mime, currentEMail.attachementsList)
        message.deletePart(mime);*/

    accept();
}

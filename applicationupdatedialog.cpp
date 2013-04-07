#include "applicationupdatedialog.h"
#include "ui_applicationupdatedialog.h"

#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QDir>
#include <QDesktopServices>
#include <QDebug>

ApplicationUpdateDialog::ApplicationUpdateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApplicationUpdateDialog)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this);
    ui->progressBarDownloadProgress->setVisible(false);
    connect(ui->pushButtonExit, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui->pushButtonDownload, SIGNAL(clicked()), this, SLOT(onDownload()));
}

ApplicationUpdateDialog::~ApplicationUpdateDialog()
{
    delete ui;
}

void ApplicationUpdateDialog::setDownloadLink(const QString &downloadLink)
{
    link.setUrl(downloadLink);
}

void ApplicationUpdateDialog::setVersion(const QString &version)
{
    QString temp = ui->labelVersion->text();
    qDebug() << temp;
    ui->labelVersion->setText(temp.arg(version));
}

void ApplicationUpdateDialog::setChangelogtext(const QString &text)
{
    ui->textEditChangelog->setHtml(text);
}

void ApplicationUpdateDialog::onDownload()
{
    QFileInfo fileInfo(link.path());
    QString fileName = fileInfo.fileName();
    filePath = QDir::tempPath();
    filePath.append(QDir::separator());

    if (fileName.isEmpty())
        fileName = "setup.exe";

    filePath.append(fileName);

    qDebug() << QDir::toNativeSeparators(filePath) << Q_FUNC_INFO;

    file = new QFile(QDir::toNativeSeparators(filePath));

    if (!file->open(QIODevice::WriteOnly))
    {
        qDebug() << "cant write file";
        delete file;
        file = 0;
        return;
    }

    ui->progressBarDownloadProgress->setValue(0);
    ui->progressBarDownloadProgress->setVisible(true);
    reply = manager->get(QNetworkRequest(link));
    connect(reply, SIGNAL(finished()),
            this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(updateDataReadProgress(qint64,qint64)));
}

void ApplicationUpdateDialog::httpFinished()
{
    file->flush();
    file->close();
    execUpdateFile();
    reply->deleteLater();
}

void ApplicationUpdateDialog::httpReadyRead()
{
    if (file)
        file->write(reply->readAll());
}

void ApplicationUpdateDialog::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes)
{
    ui->progressBarDownloadProgress->setMaximum(totalBytes);
    ui->progressBarDownloadProgress->setValue(bytesRead);
}

void ApplicationUpdateDialog::execUpdateFile()
{
    QString temp = "file:///";
    temp.append(filePath);
    qDebug() << temp << QDesktopServices::openUrl(QUrl(temp, QUrl::TolerantMode)) << Q_FUNC_INFO;
    accept();
// QDesktopServices::openUrl(QUrl("file:///C:/Documents and Settings/All Users/Desktop", QUrl::TolerantMode));
    /*QProcess* update = new QProcess(this);
    update->start(path);
    if (!update->waitForFinished())
        qDebug() << "WTF" << Q_FUNC_INFO;*/
    //qApp->quit();
    /*if (!update->waitForFinished())
        qDebug() << "failed to wait" << Q_FUNC_INFO;*/
}

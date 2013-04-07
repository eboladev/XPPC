#ifndef APPLICATIONUPDATEDIALOG_H
#define APPLICATIONUPDATEDIALOG_H

#include <QDialog>
#include <QUrl>

class QNetworkAccessManager;
class QNetworkReply;
class QFile;

namespace Ui {
class ApplicationUpdateDialog;
}

class ApplicationUpdateDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ApplicationUpdateDialog(QWidget *parent = 0);
    ~ApplicationUpdateDialog();
    void setDownloadLink(const QString& downloadLink);
    void setVersion(const QString& version);
    void setChangelogtext(const QString& text);
    
private slots:
    void onDownload();
    void httpFinished();
    void httpReadyRead();
    void updateDataReadProgress(qint64 bytesRead, qint64 totalBytes);

private:
    void execUpdateFile();

private:
    Ui::ApplicationUpdateDialog *ui;
    QNetworkAccessManager* manager;
    QNetworkReply *reply;
    QFile* file;
    QString filePath;
    QUrl link;
};

#endif // APPLICATIONUPDATEDIALOG_H

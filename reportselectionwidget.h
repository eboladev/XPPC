#ifndef REPORTSELECTIONWIDGET_H
#define REPORTSELECTIONWIDGET_H

#include <QWidget>

namespace Ui {
class ReportSelectionWidget;
}

class ReportSelectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ReportSelectionWidget(QWidget *parent = 0);
    ~ReportSelectionWidget();
    void setReportPath(const QString& path);
    void setReportInfo(const QString& text);
    QString getReportPath() const;

private Q_SLOTS:
    void onSelectReportClicked();

private:
    Ui::ReportSelectionWidget *ui;
};

#endif // REPORTSELECTIONWIDGET_H

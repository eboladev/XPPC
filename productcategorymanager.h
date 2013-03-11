#ifndef PRODUCTCATEGORYMANAGER_H
#define PRODUCTCATEGORYMANAGER_H

#include <QDialog>

#include "sqlextension.h"

class QStandardItemModel;
class QStandardItem;
class QSortFilterProxyModel;

namespace Ui {
class ProductCategoryManager;
}

class ProductCategoryManager : public QDialog, SqlExtension
{
    Q_OBJECT
    
public:
    explicit ProductCategoryManager(const QString& dbConnectionString, QWidget *parent = 0);
    ~ProductCategoryManager();

private slots:
    void onAddCategoryClicked();
    void onRemoveCategoryClicked();
    void onCurrentItemChanged(QStandardItem*);

private:
    void refreshModel();

private:
    Ui::ProductCategoryManager *ui;
    QStandardItemModel* model;
    QSortFilterProxyModel* proxy;
};

#endif // PRODUCTCATEGORYMANAGER_H

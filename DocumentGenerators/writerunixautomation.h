#ifndef WRITERUNIXAUTOMATION_H
#define WRITERUNIXAUTOMATION_H
#include <QString>

#include <stdio.h>
#include <wchar.h>

#include <sal/typesizes.h>
#include <sal/main.h>

#include <cppuhelper/bootstrap.hxx>

#include <osl/file.hxx>
#include <osl/process.h>
#include <rtl/process.h>

#include <com/sun/star/beans/XPropertySet.hpp>
#include <com/sun/star/bridge/XUnoUrlResolver.hpp>
#include <com/sun/star/frame/XComponentLoader.hpp>
#include <com/sun/star/lang/XMultiComponentFactory.hpp>
#include <com/sun/star/text/XTextField.hpp>
#include <com/sun/star/lang/XMultiServiceFactory.hpp>
#include <com/sun/star/text/XTextDocument.hpp>
#include <com/sun/star/text/XText.hpp>
#include <com/sun/star/text/XTextContent.hpp>
#include <com/sun/star/text/XBookmarksSupplier.hpp>
#include <com/sun/star/container/XNameAccess.hpp>
#include <com/sun/star/text/XTextTablesSupplier.hpp>
#include <com/sun/star/table/XCell.hpp>
#include <com/sun/star/table/XCellRange.hpp>
#include <com/sun/star/text/XTextTable.hpp>
#include <com/sun/star/container/XNamed.hpp>
#include <com/sun/star/frame/XStorable.hpp>
#include <com/sun/star/awt/XWindow.hpp>
#include <com/sun/star/table/XTableRows.hpp>
#include <com/sun/star/table/XTableColumns.hpp>
#include <com/sun/star/sdbc/XCloseable.hpp>

#include <string.h>

using namespace std;
using namespace rtl;
using namespace com::sun::star::uno;
using namespace com::sun::star::lang;
using namespace com::sun::star::beans;
using namespace com::sun::star::bridge;
using namespace com::sun::star::frame;
using namespace com::sun::star::text;
using namespace com::sun::star::container;
using namespace com::sun::star::table;
using namespace com::sun::star::awt;
using namespace com::sun::star::sdbc;
using ::rtl::OUString;

class WriterUnixAutomation
{
public:
    WriterUnixAutomation();
    ~WriterUnixAutomation();
    bool loadDocument(QString fileName,bool isHidden);
    bool closeDocument();
    bool findBookmarkByName(QString name);
    bool findTableByName(QString name);
    bool selectCellInTableByPos(const int &x, const int &y);
    bool insertText(QString text);
    bool insertTable(const int &rows, const int &columns, const QString &name);
    bool saveToFile(const QString &filePath);
    bool setVisible(const bool &visible);
    bool deleteTableByName(const QString &name);
    bool insertRows(const int &rowCount,const int &afterRow = 0);
    bool markDocumentAsSaved();
    int columnCount(const int &row);
private:
    OUString convertFileToUrl(OUString filePath);
    bool startOfficeServer();
    bool connectToServer();

private:
    Reference< XComponent > xComponent;
    Reference< XMultiComponentFactory > xMultiComponentFactoryClient_copy;
    Reference< XInterface > xDesktop;
    Reference< XTextRange > xTextRange;
    Reference< XTextTable > xTextTable;
    Reference< XInterface > xInterface;
    Reference< XComponentContext > xComponentContext;
};
#endif // WRITERUNIXAUTOMATION_H

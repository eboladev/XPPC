#include "writerunixautomation.h"
#include <QProcess>
#include <QEventLoop>
#include <QTimer>

WriterUnixAutomation::WriterUnixAutomation() :
    xComponent(0),
    xMultiComponentFactoryClient_copy(0),
    xDesktop(0),
    xTextRange(0),
    xTextTable(0),
    xInterface(0),
    xComponentContext(0)
{
    char** pChar = new char*[1];
    pChar[1] = "-env:URE_MORE_TYPES=file:///usr/lib/libreoffice/program/types/offapi.rdb";
    pChar[0] = "";
    sal_detail_initialize(2, pChar);
    startOfficeServer();
    connectToServer();
    delete pChar;
}

WriterUnixAutomation::~WriterUnixAutomation()
{
    sal_detail_deinitialize();
}

bool WriterUnixAutomation::loadDocument(QString fileName, bool isHidden)
{
    try
    {
        // gets the server component context as property of the office component factory
        Reference< XPropertySet > xPropSet( xInterface, UNO_QUERY );
        xPropSet->getPropertyValue( OUString(RTL_CONSTASCII_USTRINGPARAM("DefaultContext")) ) >>= xComponentContext;

        // gets the service manager from the office
        Reference< XMultiComponentFactory > xMultiComponentFactoryServer(
                    xComponentContext->getServiceManager() );

        /* Creates an instance of a component which supports the services specified
       by the factory. Important: using the office component context.
    */

        xDesktop = xMultiComponentFactoryServer->createInstanceWithContext(
                    OUString( RTL_CONSTASCII_USTRINGPARAM("com.sun.star.frame.Desktop" ) ),
                    xComponentContext );

        Reference < XComponentLoader > xComponentLoader(xDesktop
                                                        , UNO_QUERY );

        /* Loads a component specified by an URL into the specified new or existing
       frame.
    */

        Sequence<PropertyValue> storeProps(1);
        storeProps[0].Name = OUString::createFromAscii( "Hidden" );
        storeProps[0].Value <<= isHidden;

        xComponent = xComponentLoader->loadComponentFromURL(
                    convertFileToUrl(OUString::createFromAscii(fileName.toAscii().data())),
                    OUString( RTL_CONSTASCII_USTRINGPARAM("_blank") ), 0,
                    storeProps);

        return true;
    }
    catch(...)
    {
        return false;
    }
}

bool WriterUnixAutomation::closeDocument()
{
    try
    {
        Reference<XModel> xModel(xComponent, UNO_QUERY);

        Reference<XCloseable> xClosable (xModel, UNO_QUERY);
        if (xClosable != NULL)
            xClosable->close();
        else
            xComponent->dispose();
        sal_detail_deinitialize();
        return true;
    }
    catch(...)
    {
        return false;
    }
}

bool WriterUnixAutomation::findBookmarkByName(QString name)
{
    try
    {
        Reference<XBookmarksSupplier> xbs (xComponent, UNO_QUERY);
        ::rtl::OUString str = ::rtl::OUString(name.toLocal8Bit().data(),name.toLocal8Bit().length(), RTL_TEXTENCODING_UTF8);
        Any b = xbs->getBookmarks()->getByName(str);
        Reference<XTextContent> xtc2(b,UNO_QUERY);
        if (xtc2.is()==sal_True)
        {
            xTextRange = xtc2->getAnchor();
            return true;
        }
        else
            return false;
    }
    catch(...)
    {
        return false;
    }
}

bool WriterUnixAutomation::findTableByName(QString name)
{
    try
    {
        Reference<XTextTablesSupplier> xtts(xComponent, UNO_QUERY);
        Any xTextTableName =  xtts->getTextTables()->getByName(OUString(name.toLocal8Bit().data(),name.toLocal8Bit().length(), RTL_TEXTENCODING_UTF8));
        Reference<XTextContent> xtc(xTextTableName,UNO_QUERY);
        xTextTable = Reference<XTextTable>(xTextTableName,UNO_QUERY);
        xTextRange = xtc->getAnchor();
        selectCellInTableByPos(0,0);
        return true;
    }
    catch(...)
    {
        return false;
    }
}

bool WriterUnixAutomation::selectCellInTableByPos(const int &x, const int &y)
{
    try
    {
        Reference<XCellRange> xcr(xTextTable,UNO_QUERY);
        Reference<XText> txt(xcr->getCellByPosition(x,y),UNO_QUERY);
        xTextRange = txt->getEnd();
        return true;
    }
    catch(...)
    {
        return false;
    }
}

bool WriterUnixAutomation::insertText(QString text)
{
    try
    {
        xTextRange->setString(OUString(text.toLocal8Bit().data(),text.toLocal8Bit().length(), RTL_TEXTENCODING_UTF8));
        return true;
    }
    catch(...)
    {
        return false;
    }

}

bool WriterUnixAutomation::insertTable(const int &rows, const int &columns, const QString &name)
{
    try
    {
        Reference<XMultiServiceFactory> xmsf(xComponent,UNO_QUERY);
        Reference<XTextTable> xtt(xmsf->createInstance(OUString::createFromAscii("com.sun.star.text.TextTable")),UNO_QUERY);
        xtt->initialize(rows,columns);
        Reference<XText> xtxt(xTextRange->getText(),UNO_QUERY);
        Reference<XTextContent> xtc(xtt,UNO_QUERY);
        xtxt->insertTextContent(xTextRange,xtc,sal_False);
        Reference<XNamed> xnd(xtt,UNO_QUERY);
        xnd->setName(OUString::createFromAscii(name.toAscii().data()));
        return true;
    }
    catch(...)
    {
        return false;
    }

}

bool WriterUnixAutomation::saveToFile(const QString &filePath)
{
    try
    {
        OUString nfP = convertFileToUrl(OUString::createFromAscii(filePath.toAscii().data()));

        Reference<XStorable> xStore (xComponent, UNO_QUERY);
        Sequence<PropertyValue> storeProps(279);
        storeProps[0].Name = OUString::createFromAscii( "Author" );
        storeProps[0].Value <<= OUString::createFromAscii( "SVS" );
        storeProps[1].Name = OUString::createFromAscii("Saved");
        storeProps[1].Value <<= sal_True;
        xStore->storeToURL( nfP, storeProps );
        return true;
    }
    catch(...)
    {
        return false;
    }
}

bool WriterUnixAutomation::setVisible(const bool &visible)
{
    try
    {
        Reference<XModel> xm(xComponent,UNO_QUERY);
        Reference<XController> xc = xm->getCurrentController();
        Reference<XFrame> xf = xc->getFrame();
        Reference<XWindow> xw = xf->getContainerWindow();
        xw->setVisible(visible);
        return true;
    }
    catch(...)
    {
        return false;
    }
}

bool WriterUnixAutomation::deleteTableByName(const QString &name)
{
    try
    {
        Reference<XTextTablesSupplier> xtts(xComponent, UNO_QUERY);
        Any tableName =  xtts->getTextTables()->getByName(OUString::createFromAscii(name.toAscii().data()));
        xTextTable = Reference<XTextTable>(tableName,UNO_QUERY);
        xTextTable->dispose();
        return true;
    }
    catch(...)
    {
        return false;
    }
}

bool WriterUnixAutomation::insertRows(const int &rowCount, const int &afterRow)
{
    try
    {
        Reference<XTableRows> xrows = xTextTable->getRows();
        xrows->insertByIndex(afterRow,rowCount);
        return true;
    }
    catch(...)
    {
        return false;
    }
}

bool WriterUnixAutomation::markDocumentAsSaved()
{
    try
    {
        Sequence<PropertyValue> storeProps(1);
        storeProps[0].Name = OUString::createFromAscii( ".Saved" );
        storeProps[0].Value <<= sal_True;
        return true;
    }
    catch(...)
    {
        return false;
    }
}

int WriterUnixAutomation::columnCount(const int &row)
{
    try
    {
        Reference<XTableColumns> xcolumns = xTextTable->getColumns();
        return xcolumns->getCount();
    }
    catch(...)
    {
        return -1;
    }
}

OUString WriterUnixAutomation::convertFileToUrl(OUString filePath)
{
    OUString sAbsoluteDocUrl, sWorkingDir, sDocPathUrl;

    osl_getProcessWorkingDir(&sWorkingDir.pData);
    osl::FileBase::getFileURLFromSystemPath( filePath, sDocPathUrl);
    osl::FileBase::getAbsoluteFileURL( sWorkingDir, sDocPathUrl, sAbsoluteDocUrl);
    return sAbsoluteDocUrl;
}


bool WriterUnixAutomation::startOfficeServer()
{
    try
    {
        QString program = "soffice \"--accept=socket,host=localhost,port=2083;urp;StarOffice.ServiceManager\" --invisible --nologo";
        QProcess::startDetached (program);
        return true;
    }
    catch(...)
    {
        return false;
    }
}

bool WriterUnixAutomation::connectToServer()
{

    try
    {
        OUString sConnectionString(RTL_CONSTASCII_USTRINGPARAM("uno:socket,host=localhost,port=2083;urp;StarOffice.ServiceManager"));

        xComponentContext = Reference< XComponentContext >(::cppu::defaultBootstrap_InitialComponentContext());

        /* Gets the service manager instance to be used (or null). This method has
   been added for convenience, because the service manager is a often used
   object.
*/

        Reference< XMultiComponentFactory > xMultiComponentFactoryClient = Reference< XMultiComponentFactory >(
                    xComponentContext->getServiceManager() );

        xMultiComponentFactoryClient_copy = xMultiComponentFactoryClient;

        /* Creates an instance of a component which supports the services specified
   by the factory.
*/
        xInterface =
                xMultiComponentFactoryClient->createInstanceWithContext(
                    OUString(RTL_CONSTASCII_USTRINGPARAM("com.sun.star.bridge.UnoUrlResolver")),
                    xComponentContext );

        Reference< XUnoUrlResolver > resolver ( xInterface, UNO_QUERY );;

        // Resolves the component context from the office, on the uno URL given by argv[1].
        int i = 10; //ожидает запуска офиса 10 секунд
        while(i > 0) {
            try
            {
                QEventLoop el;
                QTimer t;
                QObject::connect(&t,SIGNAL(timeout()),&el,SLOT(quit()));
                t.setSingleShot(true);
                t.start(1000);
                el.exec();
                xInterface = Reference< XInterface >( resolver->resolve( sConnectionString ), UNO_QUERY );
                i = 0;
            }
            catch ( Exception& e )
            {
                i--;
                if(i == 0) {
                    //больше нет смысла ждать
                    printf("can not connect to server FAIL\n");
                    printf("Error: cannot establish a connection using '%s':\n       %s\n",
                           OUStringToOString(sConnectionString, RTL_TEXTENCODING_ASCII_US).getStr(),
                           OUStringToOString(e.Message, RTL_TEXTENCODING_ASCII_US).getStr());
                    return false;
                }
            }
        }
        return true;
    }
    catch(...)
    {
        return false;
    }
}

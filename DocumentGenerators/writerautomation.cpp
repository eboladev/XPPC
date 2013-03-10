#include "writerautomation.h"

WriterAutomation::WriterAutomation() :
    init(true),
    m_serviceManager(0),
    m_desktop(0),
    m_doc(0),
    m_table(0),
    m_textCursor(0)
{
    try
    {
        dhCheck(dhCreateObject(L"com.sun.star.ServiceManager", NULL, &m_serviceManager));
        dhCheck(dhGetValue(L"%o", &m_desktop, m_serviceManager, L"createInstance(%S)", L"com.sun.star.frame.Desktop"));

    } catch (std::string str)
    {
        std::cerr << str << std::endl;
    }

}

WriterAutomation::~WriterAutomation()
{
}

bool WriterAutomation::isWriterPresent()
{
    return m_desktop != 0;
}

bool WriterAutomation::findBookmarkByName(const std::wstring &name)
{
    bool retValue = true;
    LPWSTR pName = 0;
    try {
        CDispPtr bookmarksSupplier, bookmark;
        dhCheck(dhGetValue(L"%o", &bookmarksSupplier, m_doc, L"getBookmarks()"));
        pName = stringToLPWSTR(name);
        dhCheck(dhGetValue(L"%o", &bookmark, bookmarksSupplier, L"getByName(%S)", pName));
        if (bookmark)
            dhCheck(dhGetValue(L"%o", &m_textRange, bookmark, L"getAnchor()"));

    } catch (std::string str) {
        std::cerr << str << "bookmark not found " << std::endl;
        retValue = false;
    }
    if (pName)
        delete pName;
    return retValue;
}

bool WriterAutomation::findTableByName(const std::wstring &name)
{
    LPWSTR pName = 0;
    bool retValue = true;
    try {
        CDispPtr tablesSuppiler;
        dhCheck(dhGetValue(L"%o", &tablesSuppiler, m_doc, L"getTextTables()"));
        pName = stringToLPWSTR(name);

        dhCheck(dhGetValue(L"%o", &m_table, tablesSuppiler, L"getByName(%S)", pName));
        dhCheck(dhGetValue(L"%o", &m_textRange, m_table, L"getAnchor()"));
        selectCellInTableByPos(0, 0);
    } catch (std::string str) {
        std::cerr << str << std::endl;
        retValue = false;
    }
    if (pName)
        delete pName;
    return retValue;
}

bool WriterAutomation::deleteTableByName(const std::wstring &name)
{
    if (!findTableByName(name))
        return false;
    LPWSTR pName = 0;
    bool ret = false;
    try
    {
        CDispPtr tablesSuppiler;
        dhCheck(dhGetValue(L"%o", &tablesSuppiler, m_doc, L"getTextTables()"));
        pName = stringToLPWSTR(name);

        dhCheck(dhGetValue(L"%o", &m_table, tablesSuppiler, L"getByName(%S)", pName));
        dhCheck(dhCallMethod(m_table, L"dispose()"));
        ret = true;
    }
    catch(...)
    {
    }
    return ret;
}

bool WriterAutomation::insertRows(const int &rowCount, const int &beforeRow)
{
    bool ret = false;
    try
    {
        CDispPtr tableRows;
        dhCheck(dhGetValue(L"%o",&tableRows,m_table, L"getRows()"));
        dhCheck(dhCallMethod(tableRows, L"insertByIndex(%d,%d)",beforeRow,rowCount));
        ret = true;
    }
    catch(...)
    {
    }
    return ret;
}

bool WriterAutomation::selectCellInTableByPos(const int &x, const int &y)
{
    bool retValue = true;
    try {
        CDispPtr cell, xText;
        dhCheck(dhGetValue(L"%o", &cell, m_table, L"getCellByPosition(%d, %d)", x, y));
        dhCheck(dhGetValue(L"%o", &xText, cell, L"getText()"));
        dhCheck(dhGetValue(L"%o", &m_textRange, xText, L"getEnd()"));

    } catch (std::string str) {
        retValue = false;
        std::cerr << str << std::endl;
    }
    return retValue;
}


bool WriterAutomation::insertText(const std::wstring &text)
{
    LPWSTR pText = 0;
    bool retVal = true;
    try {
        pText = stringToLPWSTR(text);
        dhCheck(dhCallMethod(m_textRange, L"setString(%S)", pText));
    } catch (std::string str)
    {
        retVal = false;
        std::cerr << str << std::endl;
    }
    if (pText)
        delete pText;
    return retVal;
}

bool WriterAutomation::insertTable(const int &widht, const int &height, const std::wstring &name)
{
    LPWSTR pText = 0;
    bool retVal = true;
    try {
        pText = stringToLPWSTR(name);
        CDispPtr vCursor, vControler, oTable, oText;

        dhCheck(dhGetValue(L"%o", &vControler, m_doc, L"getCurrentController()"));
        dhCheck(dhGetValue(L"%o", &vCursor, vControler, L"getViewCursor()"));
        dhCheck(dhGetValue(L"%o", &oTable, m_doc, L"createInstance(%s)", "com.sun.star.text.TextTable"));
        //dhCheck(dhPutValue(oTable, L"BackColor = %d", 13421823));
        dhCheck(dhCallMethod(oTable, L"initialize(%d, %d)", 5, 4));
        dhCheck(dhGetValue(L"%o", &oText, m_textRange, L"getText()"));
        dhCheck(dhCallMethod(oText, L"insertTextContent (%o, %o, %b)", (IDispatch *)m_textRange, (IDispatch *)oTable, false));
        dhCheck(dhCallMethod(oTable,L"setName(%S)", pText));

    } catch (std::string str)
    {
        retVal = false;
        std::cerr << str << std::endl;
    }
    if (pText)
        delete pText;
    return retVal;
}

bool WriterAutomation::closeDocument()
{
    try {
        if (m_doc)
            dhCheck(dhCallMethod(m_doc, L"Close(%b)", false));
        m_doc = 0;
        return true;
    } catch (std::string str) {
        m_doc = 0;
        std::cerr << str << std::endl;
        return false;
    }
}

bool WriterAutomation::loadDocument(std::wstring fileName, const bool &isHidden)
{
    WCHAR *url = 0;
    try {
        if (fileName != L"private:factory/swriter")
            fileName = convertFileToURL(fileName);
        VARIANT arr;
        VariantInit(&arr);
        arr.vt = VT_ARRAY | VT_DISPATCH;
        arr.parray = SafeArrayCreateVector( VT_DISPATCH, 0, 1 );
        CDispPtr propValue;
        getPropertyValue(&propValue);
        dhCheck(dhPutValue(propValue, L"Name = %S", L"Hidden"));
        dhCheck(dhPutValue(propValue, L"Value = %b", isHidden));
        long ind = 0;
        SafeArrayPutElement(arr.parray, &ind, propValue);
        url = stringToLPWSTR(fileName);
        dhCheck(dhGetValue(L"%o", &m_doc, m_desktop, L"LoadComponentFromURL(%S, %S, %u, %v)",
                           url, L"_blank",
                           0L, &arr));
        CDispPtr text;
        dhCheck(dhGetValue(L"o", &text, m_doc, L"getText()"));
        dhCheck(dhGetValue(L"o", &m_textRange, text, L"getEnd()"));
        return true;
    } catch (std::string str) {
        if (url)
            delete url;
        std::cerr << str << std::endl;
        return false;
    }
}

bool WriterAutomation::saveToFile(const std::wstring &filePath)
{
    bool retValue = true;
    LPWSTR pFilePath = 0;
    try {
        std::wstring nFilePath = convertFileToURL(filePath);
        pFilePath = stringToLPWSTR(nFilePath);
        VARIANT arr;
        VariantInit(&arr);
        arr.parray = SafeArrayCreateVector(VT_DISPATCH, 0, 1);
        arr.vt = VT_ARRAY | VT_DISPATCH;
        CDispPtr propValue;
        getPropertyValue(&propValue);
        dhCheck(dhPutValue(propValue, L"Name = %S", L"Author"));
        dhCheck(dhPutValue(propValue, L"Value = %S", L"SVS"));

        long ind = 0;
        SafeArrayPutElement(arr.parray, &ind, propValue);
        dhCheck(dhCallMethod(m_doc, L"storeAsURL(%S, %v)", pFilePath, &arr));
    } catch (std::string str) {
        std::cerr << str << std::endl;
        retValue = false;
    }
    if (pFilePath)
        delete pFilePath;
    return retValue;
}

bool WriterAutomation::setVisible(const bool &visible)
{
    try {
        CDispPtr frame, controller, windows;
        dhCheck(dhGetValue(L"%o", &controller, m_doc, L"getCurrentController()"));
        dhCheck(dhGetValue(L"%o", &frame, controller, L"getFrame()"));
        dhCheck(dhGetValue(L"%o", &windows, frame, L"getContainerWindow()"));
        dhCheck(dhCallMethod(windows, L"setVisible(%b)", visible));
        return true;
    } catch (std::string str) {
        std::cerr << str << std::endl;
        return false;
    }
}

std::wstring WriterAutomation::convertFileToURL(const std::wstring &fileName)
{
    std::wstring URL=L"file://localhost/";
    for (int i=0; i < fileName.length(); ++i)
        if (fileName.at(i) == ' ') URL += L"%20";
        else if (fileName[i]=='\\') URL += L"/";
        else URL+=fileName.at(i);
    return URL;
}

bool WriterAutomation::getPropertyValue(void *pResult)
{
    try {
        dhCheck(dhGetValue(L"o", pResult, m_serviceManager, L"Bridge_GetStruct(%S)", L"com.sun.star.beans.PropertyValue"));
        return true;
    } catch (std::string str) {
        std::cerr << str << std::endl;
        return false;
    }
}

LPWSTR WriterAutomation::stringToLPWSTR(const std::wstring &string)
{
    WCHAR *ret = new WCHAR[string.length() + 1];
    memset(ret, 0, (string.length() + 1) * sizeof(WCHAR));
    string.copy(ret, string.length(), 0);
    return ret;
}

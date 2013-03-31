#include "wordautomation.h"
#include <QDir>
#include <QDebug>

WordAutomation::WordAutomation() :
    m_word(0),
    m_wordDoc(0),
    m_quitFromWordAutomaticaly(false)
{
    try
    {
        dhCheck(dhCreateObject(L"Word.Application", NULL, &m_word));
        dhCheck(dhPutValue(m_word, L"Visible = %b", false));
    } catch (...)
    {

    }
}

WordAutomation::~WordAutomation()
{
    if (m_quitFromWordAutomaticaly)
    {
        markDocumentAsSaved();
        quit();
    }
}

bool WordAutomation::isWordPresent()
{
    return m_word != 0;
}

bool WordAutomation::findBookmarkByName(const QString &name)
{
    bool ret = false;
    LPWSTR p_name = QStringToLPWSTR(QDir::toNativeSeparators(name));
    try {
        CDispPtr bookmarks;
        dhCheck(dhGetValue(L"%o", &bookmarks, m_wordDoc, L"Bookmarks()"));
        BOOL bookmarkExist;
        dhCheck(dhGetValue(L"%b", &bookmarkExist, bookmarks, L"Exists(%S)", p_name));
        if (bookmarks)
        {
            CDispPtr bookmark;
            dhCheck(dhGetValue(L"%o", &bookmark, bookmarks, L"Item(%S)", p_name));
            dhCheck(dhGetValue(L"%o", &m_range, bookmark, L"Range()"));
        }
        ret = bookmarkExist;
    } catch (...) {
    }
    delete p_name;
    return ret;
}

bool WordAutomation::findTableByName(const QString &name)
{
    if (!findBookmarkByName(name))
        return false;
    bool ret = false;
    try {
        dhCheck(dhGetValue(L"%o", &m_table, m_range , L"Tables(%d)", 1));
        ret =true;
    } catch (...) {
    }
    return ret;
}

bool WordAutomation::deleteTableByName(const QString &name)
{
    if (!findTableByName(name))
        return false;
    bool ret = false;
    try {
        dhCheck(dhCallMethod(m_table, L"Delete"));
        m_table = 0;
        ret = true;
    } catch (...) {
    }
    return ret;
}

bool WordAutomation::selectCellInTableByPos(const int &column, const int &row)
{
    CDispPtr cell;
    bool ret = false;
    try {
        dhCheck(dhGetValue(L"%o", &cell, m_table , L"Cell(%d, %d)", row, column));
        dhCheck(dhGetValue(L"%o", &m_range, cell , L"Range()"));
        ret = true;
    } catch (...) {
    }
    return ret;
}

bool WordAutomation::loadDocument(const QString &fileName, const bool &isHidden)
{    
    LPWSTR p_name = QStringToLPWSTR(QDir::toNativeSeparators(fileName));
    bool ret = false;
    try {        
        dhCheck(dhGetValue(L"%o", &m_wordDocs, m_word, L"Documents()"));        
        dhCheck(dhGetValue(L"%o", &m_wordDoc, m_wordDocs, L"Add(%S)", p_name));        
        dhCheck(dhPutValue(m_word, L"Visible = %b", !isHidden));        
        ret = true;        
    } catch (...) {
    }
    delete p_name;
    return ret;
}

int WordAutomation::columnCount(const int &row)
{
    int ret = -1;
    try {
        int columnCount;
        dhCheck(dhGetValue(L"%d", &columnCount, m_table, L"Rows().Item(%d).Cells().Count()", row));
        ret = columnCount;
    } catch (...) {
    }
    return ret;
}

bool WordAutomation::insertRows(const int &rowCount, const int &beforeRow)
{
    bool ret = false;
    try {
        CDispPtr p_row, p_rows;
        dhCheck(dhGetValue(L"o", &p_rows, m_table, L"Rows"));
        if (beforeRow > 0)
            dhCheck(dhGetValue(L"o", &p_row, p_rows, L"Item(%d)", beforeRow));
        for (int i = 0; i < rowCount; ++i)
            if (beforeRow > 0)
                dhCheck(dhCallMethod(p_rows, L"Add(%o)", (IDispatch*)p_row));
            else
                dhCheck(dhCallMethod(p_rows, L"Add()"));


        ret = true;
    } catch (...) {
    }
    return ret;
}

bool WordAutomation::markDocumentAsSaved()
{
    bool ret = false;
    try {
        dhPutValue(m_wordDoc, L".Saved = %b", TRUE);
        ret = true;
    } catch (...) {
    }
    return ret;
}

bool WordAutomation::closeDocument()
{
    bool ret = false;
    try {
        dhCheck(dhCallMethod(m_wordDoc, L"Close()"));
        dhCheck(dhPutValue(m_word, L"Visible = %b", false));
        ret = true;
    } catch (...) {
    }
    return ret;
}

bool WordAutomation::quit()
{
    bool ret = false;
    try {
        dhCheck(dhCallMethod(m_word, L"Quit()"));
        ret = true;
    } catch (...) {
    }
    return ret;
}


LPWSTR QStringToLPWSTR(const QString &string)
{
    LPWSTR p_ret = new WCHAR[string.length() + 1];
    memset(p_ret, 0, (string.length() + 1)  * sizeof(WCHAR));
    string.toWCharArray(p_ret);
    return p_ret;
}


bool WordAutomation::saveToFile(const QString &filePath)
{
    bool ret = false;
    LPWSTR p_name = QStringToLPWSTR(QDir::toNativeSeparators(filePath));
    try {
        dhCheck(dhCallMethod(m_wordDoc, L"SaveAs(%S)", p_name));
        ret = true;
    } catch (...) {
    }
    delete p_name;
    return ret;
}

bool WordAutomation::setVisible(const bool &visible)
{
    bool ret = false;
    try {
        dhCheck(dhPutValue(m_word, L"Visible = %b", visible));
        ret = true;
    } catch (...) {
    }
    return ret;
}


bool WordAutomation::insertText(const QString &text)
{
    LPWSTR p_text = QStringToLPWSTR(text);
    bool ret = false;
    try {
        dhCheck(dhCallMethod(m_range, L"InsertAfter(%S)", p_text));
        ret = true;
    } catch (...) {
    }
    delete p_text;
    return ret;
}

bool WordAutomation::insertTable(const int &widht, const int &height, const QString &name)
{
    bool ret = false;
    try {
        //CDispPtr tables;
        dhCheck(dhGetValue(L"%o", &m_table, m_wordDoc, L".Tables.Add(%o, %d, %d, %d)", (IDispatch *)m_range, height, widht, 1));
        ret = true;
    } catch (...) {
    }
    return ret;
}


ExcelAutomation::ExcelAutomation() :
    m_excel(0),
    m_workbooks(0),
    m_quitFromExcelAutomaticaly(false)
{
    try
    {
        dhCheck(dhCreateObject(L"Excel.Application", NULL, &m_excel));
        dhCheck(dhPutValue(m_excel, L"Visible = %b", false));
        dhCheck(dhGetValue(L"%o", &m_workbooks, m_excel, L"Workbooks"));

    } catch (...)
    {
        m_excel = 0;
        m_workbooks = 0;
    }
}

ExcelAutomation::~ExcelAutomation()
{
    if (m_quitFromExcelAutomaticaly)
    {
        markDocumentAsSaved();
        quit();
    }

}

bool ExcelAutomation::markDocumentAsSaved()
{
    bool ret = false;
    try {
        dhPutValue(m_excel, L".Saved = %b", TRUE);
        ret = true;
    } catch (...) {
    }
    return ret;
}

void ExcelAutomation::setQuitAutomaticaly(const bool &quit)
{
    m_quitFromExcelAutomaticaly = quit;
}

bool ExcelAutomation::quit()
{
    bool ret = false;
    try {
        dhCheck(dhCallMethod(m_excel, L"Quit()"));
        ret = true;
    } catch (...) {
    }
    return ret;
}

bool ExcelAutomation::excelIsPresent()
{
    return m_excel != 0;
}

QString ExcelAutomation::getCellValue(const int &row, const int &col)
{
    QString res;
    try {
        CDispPtr cell;
        CDhStringW str;
        dhCheck(dhGetValue(L"%o", &cell, m_sheet, L"Cells(%d, %d)", row, col));
        dhCheck(dhGetValue(L"%S", &str, cell, L"Value()"));
        res = QString::fromWCharArray(str);

    } catch (...) {
    }
    return res;
}

bool ExcelAutomation::loadDocument(const QString &filename, const bool &isVisible = false)
{
    LPWSTR p_name = QStringToLPWSTR(QDir::toNativeSeparators(filename));
    bool ret = false;
    try {
        dhCheck(dhGetValue(L"%o", &m_workbook, m_workbooks, L"Open(%S)", p_name));
        qDebug() << 1;
        dhCheck(dhGetValue(L"%o", &m_sheets, m_workbook, L"Sheets()"));
        qDebug() << 2;
        dhCheck(dhGetValue(L"%o", &m_sheet, m_sheets, L"Item(%d)", 1));
        qDebug() << 3;
        ret = true;
    } catch (...) {
    }
    delete p_name;
    return ret;
}

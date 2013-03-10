#ifndef WORDAUTOMATION_H
#define WORDAUTOMATION_H
#include "disphelper.h"
#include <QString>

LPWSTR QStringToLPWSTR(const QString &string);

class WordAutomation
{
public:
    WordAutomation();
    ~WordAutomation();
    bool isWordPresent();
    bool findBookmarkByName(const QString &name);
    bool findTableByName(const QString &name);
    bool deleteTableByName(const QString &name);//
    bool selectCellInTableByPos(const int &column, const int &row);
    bool insertText(const QString &text);
    bool insertTable(const int &widht, const int &height, const QString &name);
    bool loadDocument(const QString &fileName, const bool &isHidden = false);
    int columnCount(const int &row);
    bool insertRows(const int &rowCount, const int &beforeRow = -1);//
    bool markDocumentAsSaved();//
    bool closeDocument();
    bool quit();
    void setQuitFromWordAutomaticaly(const bool &enabled) {m_quitFromWordAutomaticaly = enabled;}
    bool saveToFile(const QString &filePath);
    bool setVisible(const bool &visible);

private:
    bool m_quitFromWordAutomaticaly;
    CDhInitialize init;
    CDispPtr m_word;
    CDispPtr m_range;
    CDispPtr m_wordDoc;
    CDispPtr m_wordDocs;
    CDispPtr m_table;
};

class ExcelAutomation
{
public:
    ExcelAutomation();
    ~ExcelAutomation();
    bool markDocumentAsSaved();
    void setQuitAutomaticaly(const bool &quit);
    bool quit();
    bool excelIsPresent();
    QString getCellValue(const int &row, const int &col);
    bool loadDocument(const QString &filename, const bool &isVisible);

private:
    bool m_quitFromExcelAutomaticaly;
    CDispPtr m_excel;
    CDispPtr m_workbooks;
    CDispPtr m_workbook;
    CDispPtr m_sheets;
    CDispPtr m_sheet;
    CDhInitialize init;


};

#endif // WORDAUTOMATION_H

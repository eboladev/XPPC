#ifndef WRITERAUTOMATION_H
#define WRITERAUTOMATION_H
#include "disphelper.h"

class WriterAutomation
{
public:
    explicit WriterAutomation();
    ~WriterAutomation();
    bool isWriterPresent();
    bool findBookmarkByName(const std::wstring &name);
    bool findTableByName(const std::wstring &name);
    bool selectCellInTableByPos(const int &x, const int &y);
    bool insertText(const std::wstring &text);
    bool insertTable(const int &widht, const int &height, const std::wstring &name);
    bool loadDocument(std::wstring fileName = L"private:factory/swriter", const bool &isHidden = false);
    bool closeDocument();
    bool saveToFile(const std::wstring &filePath);
    bool setVisible(const bool &visible);
    bool deleteTableByName(const std::wstring &name);
    bool insertRows(const int &rowCount,const int &beforeRow = -1);
    //bool markDocumentAsSaved();

private:
    std::wstring convertFileToURL(const std::wstring &fileName);
    bool getPropertyValue(void *pResult);
    LPWSTR stringToLPWSTR(const std::wstring &string);


private:
    CDhInitialize init;
    CDispPtr m_serviceManager;
    CDispPtr m_desktop;
    CDispPtr m_doc;
    CDispPtr m_table;
    CDispPtr m_textCursor;
    CDispPtr m_textRange;
};

#endif // WRITERAUTOMATION_H

#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <comdef.h>

class WordProcessor {
private:
    CLSID clsid;
    IDispatch* pWordApp;
    IDispatch* pDocuments;
    IDispatch* pActiveDocument;
    bool isInitialized;

    bool initializeCOM();
    bool createWordInstance();
    bool openCurrentDocument();
    BSTR convertToBSTR(const std::string& str);
    std::string convertFromBSTR(BSTR bstr);

public:
    WordProcessor();
    ~WordProcessor();

    bool initialize();
    std::string getDocumentText();
    void replaceText(const std::string& oldText, const std::string& newText);
    void createAbbreviationList(const std::vector<std::pair<std::string, std::string>>& abbreviations);
    void cleanup();
};

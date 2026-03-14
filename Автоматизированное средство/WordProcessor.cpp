#include "WordProcessor.h"
#include <iostream>
#include <comdef.h>
#include <regex>

WordProcessor::WordProcessor() : pWordApp(nullptr), pDocuments(nullptr),
pActiveDocument(nullptr), isInitialized(false) {
    std::cout << "WordProcessor создан" << std::endl;
}

WordProcessor::~WordProcessor() {
    cleanup();
}

bool WordProcessor::initializeCOM() {
    HRESULT hr = CoInitialize(nullptr);
    return SUCCEEDED(hr);
}

bool WordProcessor::createWordInstance() {
    HRESULT hr = CLSIDFromProgID(L"Word.Application", &clsid);
    if (FAILED(hr)) return false;

    hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER,
        IID_IDispatch, (void**)&pWordApp);
    return SUCCEEDED(hr);
}

bool WordProcessor::openCurrentDocument() {
    // Временная заглушка для тестирования
    std::cout << "Открытие документа Word..." << std::endl;
    return true;
}

BSTR WordProcessor::convertToBSTR(const std::string& str) {
    int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    wchar_t* wsz = new wchar_t[len];
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wsz, len);
    BSTR bstr = SysAllocString(wsz);
    delete[] wsz;
    return bstr;
}

std::string WordProcessor::convertFromBSTR(BSTR bstr) {
    if (!bstr) return "";
    int len = WideCharToMultiByte(CP_UTF8, 0, bstr, -1, NULL, 0, NULL, NULL);
    char* sz = new char[len];
    WideCharToMultiByte(CP_UTF8, 0, bstr, -1, sz, len, NULL, NULL);
    std::string str(sz);
    delete[] sz;
    return str;
}

bool WordProcessor::initialize() {
    std::cout << "Инициализация WordProcessor..." << std::endl;

    if (!initializeCOM()) {
        std::cerr << "Ошибка инициализации COM" << std::endl;
        return false;
    }

    if (!createWordInstance()) {
        std::cerr << "Ошибка создания экземпляра Word" << std::endl;
        return false;
    }

    if (!openCurrentDocument()) {
        std::cerr << "Ошибка открытия документа" << std::endl;
        return false;
    }

    isInitialized = true;
    std::cout << "WordProcessor успешно инициализирован" << std::endl;
    return true;
}

std::string WordProcessor::getDocumentText() {
    // Временная заглушка для тестирования
    return "Министерство внутренних дел (МВД) опубликовало отчет. МВД сообщило о новых инициативах. Также были рассмотрены вопросы ФСБ (Федеральной службы безопасности). Далее по тексту используется аббревиатура ООН - Организация Объединенных Наций.";
}

void WordProcessor::replaceText(const std::string& oldText, const std::string& newText) {
    std::cout << "Замена '" << oldText << "' на '" << newText << "'" << std::endl;
}

void WordProcessor::createAbbreviationList(const std::vector<std::pair<std::string, std::string>>& abbreviations) {
    std::cout << "Создание списка сокращений:" << std::endl;
    for (const auto& abbr : abbreviations) {
        std::cout << abbr.first << " - " << abbr.second << std::endl;
    }
}

void WordProcessor::cleanup() {
    std::cout << "Очистка ресурсов WordProcessor" << std::endl;
    if (pWordApp) {
        pWordApp->Release();
        pWordApp = nullptr;
    }
    CoUninitialize();
}
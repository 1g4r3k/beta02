#include <iostream>
#include <windows.h>
#include <locale>
#include <memory>
#include <chrono>
#include "WordProcessor.h"
#include "AbbreviationDetector.h"

int main() {
    // Настройка кодировки для русского языка
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");

    std::cout << "=== Автоматизированное средство формирования списка сокращений ===" << std::endl;
    std::cout << "Версия 1.0" << std::endl << std::endl;

    try {
        auto start = std::chrono::high_resolution_clock::now();

        // Создание объектов
        auto wordProc = std::make_unique<WordProcessor>();
        auto detector = std::make_unique<AbbreviationDetector>();

        // Инициализация
        if (!wordProc->initialize()) {
            std::cerr << "Ошибка инициализации Microsoft Word" << std::endl;
            return 1;
        }

        std::cout << "Подключение к Word успешно!" << std::endl;

        // Получение текста
        std::string text = wordProc->getDocumentText();
        std::cout << "Текст документа загружен. Длина: " << text.length() << " символов" << std::endl;

        // Поиск сокращений
        auto abbreviations = detector->findAbbreviations(text);
        std::cout << "Найдено сокращений: " << abbreviations.size() << std::endl;

        // Сортировка
        auto sorted = detector->sortAbbreviationsAlphabetically(abbreviations);

        // Создание списка
        wordProc->createAbbreviationList(sorted);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Время выполнения: " << duration.count() / 1000.0 << " сек." << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "\nНажмите Enter для выхода...";
    std::cin.get();

    return 0;
}
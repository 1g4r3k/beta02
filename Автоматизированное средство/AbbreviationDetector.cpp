#include "AbbreviationDetector.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <regex>
#include <iostream>

// Реализация normalizeAbbreviation
std::string AbbreviationDetector::normalizeAbbreviation(const std::string& abbr) {
    std::string result = abbr;
    // Удаление пробелов и лишних символов
    result.erase(std::remove_if(result.begin(), result.end(), ::isspace), result.end());

    // Приведение к верхнему регистру
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);

    return result;
}

// Реализация normalizeDefinition
std::string AbbreviationDetector::normalizeDefinition(const std::string& def) {
    std::string result = def;

    // Удаление лишних пробелов в начале и конце
    size_t start = result.find_first_not_of(" \t\n\r");
    size_t end = result.find_last_not_of(" \t\n\r");

    if (start == std::string::npos || end == std::string::npos) {
        return "";
    }

    result = result.substr(start, end - start + 1);

    // Замена множественных пробелов одним
    std::string normalized;
    bool previousSpace = false;

    for (char c : result) {
        if (std::isspace(c)) {
            if (!previousSpace) {
                normalized += ' ';
                previousSpace = true;
            }
        }
        else {
            normalized += c;
            previousSpace = false;
        }
    }

    return normalized;
}

// Реализация isAbbreviation
bool AbbreviationDetector::isAbbreviation(const std::string& word) {
    if (word.length() < 2) return false;

    int upperCount = 0;
    for (char c : word) {
        if (isupper(static_cast<unsigned char>(c)) ||
            (c >= 'А' && c <= 'Я')) {
            upperCount++;
        }
    }

    return (static_cast<double>(upperCount) / word.length() >= 0.7);
}

// Реализация findComplexAbbreviations
void AbbreviationDetector::findComplexAbbreviations(const std::string& text,
    std::map<std::string, std::string>& abbreviations) {
    // Контекстный анализ для сложных случаев
    std::istringstream iss(text);
    std::string line;

    while (std::getline(iss, line)) {
        // Поиск паттернов типа "далее - АББР"
        std::regex pattern(R"(([а-яА-Яa-zA-Z\s]+?)\s+далее\s*[-–—]\s*([A-ZА-Я]{2,}))");
        std::smatch match;

        if (std::regex_search(line, match, pattern)) {
            std::string abbr = match[2].str();
            std::string definition = match[1].str();

            abbr = normalizeAbbreviation(abbr);
            definition = normalizeDefinition(definition);

            if (abbreviations.find(abbr) == abbreviations.end()) {
                abbreviations[abbr] = definition;
            }
        }
    }
}

// Реализация compareRussianStrings
int AbbreviationDetector::compareRussianStrings(const std::string& a, const std::string& b) {
    // Простая лексикографическая сортировка
    // Для более точной сортировки с учетом русской локали используйте setlocale
    return a.compare(b);
}

// Реализация findAbbreviations
std::map<std::string, std::string> AbbreviationDetector::findAbbreviations(const std::string& text) {
    std::map<std::string, std::string> abbreviations;

    // Паттерны для поиска сокращений
    std::vector<std::regex> patterns = {
        // Формат: "Аббревиатура (Расшифровка)"
        std::regex(R"(([A-ZА-Я]{2,})\s*\(([^)]+)\))"),

        // Формат: "Расшифровка (Аббревиатура)"
        std::regex(R"(([а-яА-Яa-zA-Z\s]+?)\s*\(([A-ZА-Я]{2,})\))"),

        // Формат: "Аббревиатура - Расшифровка"
        std::regex(R"(([A-ZА-Я]{2,})\s*[-–—]\s*([а-яА-Яa-zA-Z\s]+))"),

        // Поиск в сносках (специальный формат)
        std::regex(R"(Примечание:\s*([A-ZА-Я]{2,})\s*[-–—]\s*([^\.]+))")
    };

    std::string::const_iterator searchStart(text.cbegin());

    for (const auto& pattern : patterns) {
        std::smatch match;
        std::string::const_iterator iter = searchStart;

        while (std::regex_search(iter, text.cend(), match, pattern)) {
            std::string abbr, definition;

            // Определяем, где аббревиатура, а где расшифровка
            if (isAbbreviation(match[1].str())) {
                abbr = match[1].str();
                definition = match[2].str();
            }
            else if (isAbbreviation(match[2].str())) {
                abbr = match[2].str();
                definition = match[1].str();
            }
            else {
                iter = match.suffix().first;
                continue;
            }

            // Очистка и нормализация
            abbr = normalizeAbbreviation(abbr);
            definition = normalizeDefinition(definition);

            // Добавляем только если это не повтор
            if (abbreviations.find(abbr) == abbreviations.end()) {
                abbreviations[abbr] = definition;
            }

            iter = match.suffix().first;
        }
    }

    // Дополнительный поиск сложных случаев
    findComplexAbbreviations(text, abbreviations);

    return abbreviations;
}

// Реализация sortAbbreviationsAlphabetically
std::vector<std::pair<std::string, std::string>>
AbbreviationDetector::sortAbbreviationsAlphabetically(const std::map<std::string, std::string>& abbreviations) {
    std::vector<std::pair<std::string, std::string>> sorted;

    for (const auto& item : abbreviations) {
        sorted.push_back(item);
    }

    // Сортировка по алфавиту
    std::sort(sorted.begin(), sorted.end(),
        [this](const auto& a, const auto& b) {
            return compareRussianStrings(a.first, b.first) < 0;
        });

    return sorted;
}
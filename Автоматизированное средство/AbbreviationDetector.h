#pragma once
#pragma once
#include <string>
#include <vector>
#include <map>
#include <regex>

class AbbreviationDetector {
private:
    bool isAbbreviation(const std::string& word);
    std::string normalizeAbbreviation(const std::string& abbr);
    std::string normalizeDefinition(const std::string& def);
    void findComplexAbbreviations(const std::string& text,
        std::map<std::string, std::string>& abbreviations);
    int compareRussianStrings(const std::string& a, const std::string& b);

public:
    AbbreviationDetector() = default;

    std::map<std::string, std::string> findAbbreviations(const std::string& text);
    std::vector<std::pair<std::string, std::string>>
        sortAbbreviationsAlphabetically(const std::map<std::string, std::string>& abbreviations);
};
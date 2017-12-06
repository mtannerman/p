#pragma once

#include <vector>
#include <string>
#include <memory>
#include <istream>

class CSVParser
{
public:
    CSVParser(std::istream& arg_istream, const char arg_delimiter = ',', const char arg_quotation = '\"');
    bool readRecord(std::vector<std::string>& record);
private:
    char GetCurrentCharacter();
    char GetNextCharacter();

    std::istream& mStream;
    const char mDelimiter;
    const char mQuotation;
};
#pragma once

#include <vector>
#include <string>
#include <memory>
#include <istream>

#include <sstream>
#define STR(msg) (((::std::ostringstream*)&( ::std::ostringstream() << "" << msg))->str())
#define LOG(msg) std::cout << STR(msg) << std::endl;

class CSVParser
{
public:
    CSVParser(std::istream& arg_istream, const char arg_delimiter = ',', const char arg_quotation = '\"');
    bool readRecord(std::vector<std::string>& record, const bool doLogging = false);
private:
    char GetCurrentCharacter();
    char GetNextCharacter();

    std::istream& mStream;
    const char mDelimiter;
    const char mQuotation;
};
#pragma once

#include <vector>
#include <string>
#include <memory>
#include <istream>

#include <sstream>
#define STR(msg) (((::std::ostringstream*)&( ::std::ostringstream() << "" << msg))->str())

class CSVParser
{
public:
    CSVParser(std::istream& arg_istream, const char arg_delimiter = ',', const char arg_quotation = '\"');
    bool readRecord(std::vector<std::string>& record, std::vector<std::string>& log);
private:
    char GetCurrentCharacter();
    char GetNextCharacter();

    std::istream& mStream;
    const char mDelimiter;
    const char mQuotation;
};
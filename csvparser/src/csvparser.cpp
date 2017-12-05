#include "csvparser/csvparser.h"

namespace parser_detail
{
    
}

CSVParser::CSVParser(std::istream& arg_istream, const char arg_delimiter, const char arg_quotation)
    : mStream(arg_istream)
    , mDelimiter(arg_delimiter)
    , mQuotation(arg_quotation)
{}

enum class FieldState { UNQUOTED, QUOTED, QUOTE_IN_QUOTE };

bool CSVParser::readRecord(std::vector<std::string>& record)
{
    std::string line;
    if (std::getline(mStream, line)) {

    }
    return false;
}
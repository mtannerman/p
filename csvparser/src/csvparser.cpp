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
    record.clear();
    // auto state = std::ios_base::goodbit;
    const std::istream::sentry streamSentry(mStream, true);
    const bool isStreamOk = bool(streamSentry);

    if (isStreamOk) {
        try {
            for (auto currentCharacter = GetCurrentCharacter(); ; currentCharacter = GetNextCharacter()) {
                if (std::char_traits<char>::eq_int_type(currentCharacter, std::char_traits<char>::eof())) {
                    mStream.setstate(std::ios_base::eofbit);
                    break;
                }
            }
        }
        catch (...) {
            mStream.setstate(std::ios_base::badbit);
        }
    }
    else {
        // throw
    }


    return false;
}

char CSVParser::GetCurrentCharacter()
{
    return mStream.rdbuf()->sgetc();
}

char CSVParser::GetNextCharacter()
{
    return mStream.rdbuf()->snextc();
}
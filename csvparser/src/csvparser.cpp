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
            std::string currentWord;
            auto fieldState = FieldState::UNQUOTED;
            for (auto currentCharacter = GetCurrentCharacter(); ; currentCharacter = GetNextCharacter()) {
                if (std::char_traits<char>::eq_int_type(currentCharacter, std::char_traits<char>::eof())) {
                    mStream.setstate(std::ios_base::eofbit);
                    if (!currentWord.empty()) {
                        record.push_back(currentWord);
                        currentWord = "";
                    }
                    return true;
                }
                else if (currentCharacter == '\n') {
                    if (fieldState == FieldState::QUOTE_IN_QUOTE) {
                        currentWord += currentCharacter;
                    }
                    else {
                        record.push_back(currentWord);
                        currentWord = "";
                        GetNextCharacter();
                        return true;
                    }
                }
                else if (currentCharacter == '\r') {
                     if (fieldState == FieldState::QUOTE_IN_QUOTE) {
                        currentWord += '\n';
                    }
                    else {
                        record.push_back(currentWord);
                        currentWord = "";
                        const auto nextCharacter = GetNextCharacter();
                        if (nextCharacter == '\n') {
                            GetNextCharacter();
                        }
                        return true;
                    }
                }
                else if (currentCharacter == mDelimiter) {
                    if (fieldState == FieldState::QUOTED) {
                        currentWord += currentCharacter;
                    }
                    else {
                        record.push_back(currentWord);
                        currentWord = "";
                    }
                }
                else if (currentCharacter == mQuotation) {
                    if (fieldState == FieldState::QUOTED) {
                        fieldState = FieldState::QUOTE_IN_QUOTE;
                    }
                    else if (fieldState == FieldState::UNQUOTED) {
                        fieldState = FieldState::QUOTED;
                    }
                    else {
                        currentWord += currentCharacter;
                        fieldState = FieldState::UNQUOTED;
                    }
                }
                else {
                    currentWord += currentCharacter;
                }
            }
        }
        catch (...) {
            mStream.setstate(std::ios_base::badbit);
        }
    }
    // THROW

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
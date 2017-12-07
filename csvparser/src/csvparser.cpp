#include "../../csvparser/inc/csvparser.h"
#include <stdexcept>
#include <iostream>

#define LOG_IF(msg) if (doLogging) LOG(msg) 

CSVParser::CSVParser(std::istream& arg_istream, const char arg_delimiter, const char arg_quotation)
    : mStream(arg_istream)
    , mDelimiter(arg_delimiter)
    , mQuotation(arg_quotation)
{}

enum class FieldState { UNQUOTED, QUOTED, QUOTE_IN_QUOTE };
std::string FSSTR(const FieldState f)
{
	switch (f)
	{
		case FieldState::UNQUOTED:
			return "  UNQUOTED    ";
		case FieldState::QUOTED:
			return "   QUOTED     ";
		case FieldState::QUOTE_IN_QUOTE:
			return "QUOTE_IN_QUOTE";
	}
	return "INVALID";
}

std::string CWS(const std::string& w) {
    std::string ret;
    for (auto c : w) {
        if (c == '\n') {
            ret += "\\n";
        }
        else {
            ret += c;
        }
    }
    return ret;
}

bool CSVParser::readRecord(std::vector<std::string>& record, const bool doLogging)
{
    record.clear();
    // auto state = std::ios_base::goodbit;
    const std::istream::sentry streamSentry(mStream, true);
    const bool isStreamOk = bool(streamSentry);
    if (isStreamOk) {
        std::string currentWord;
        auto fieldState = FieldState::UNQUOTED;
        for (auto currentCharacter = GetCurrentCharacter(); ; currentCharacter = GetNextCharacter()) {
            if (std::char_traits<char>::eq_int_type(currentCharacter, std::char_traits<char>::eof())) {
                mStream.setstate(std::ios_base::eofbit);
                if (!currentWord.empty()) {
                    record.push_back(currentWord);
                    currentWord = "";
					LOG_IF("[eof][" + FSSTR(fieldState) << "][" << CWS(currentWord) << "]");
                }
                if (fieldState != FieldState::UNQUOTED) {
                    throw std::invalid_argument("Quote mismatch.");
                }
                return true;
            }
            else if (currentCharacter == '\n') {
                if (fieldState == FieldState::QUOTE_IN_QUOTE) {
                    record.push_back(currentWord);
                    // currentWord += currentCharacter;
					LOG_IF("[\\n][" + FSSTR(fieldState) << "][" << CWS(currentWord) << "]");
                    GetNextCharacter();
                    return true;
                }
                else if (fieldState == FieldState::QUOTED) {
                    currentWord =+ currentCharacter;
					LOG_IF("[\\n][" + FSSTR(fieldState) << "][" << CWS(currentWord) << "]");
                }
                else {
                    LOG_IF("[\\n][" + FSSTR(fieldState) << "][" << CWS(currentWord) << "]");
					record.push_back(currentWord);
                    currentWord = "";
                    GetNextCharacter();
                    return true;
                }
            }
            else if (currentCharacter == '\r') {
                 if (fieldState == FieldState::QUOTE_IN_QUOTE) {
					 currentWord += '\n';
					 LOG_IF("[\\r][" + FSSTR(fieldState) << "][" << CWS(currentWord) << "]");
                }
                else {
                    record.push_back(currentWord);
                    currentWord = "";
					LOG_IF("[\\r][" + FSSTR(fieldState) << "][" << CWS(currentWord) << "]");
                    const auto nextCharacter = GetNextCharacter();
                    if (nextCharacter == '\n') {
						LOG_IF("next is n");
                        GetNextCharacter();
                    }
                    return true;
                }
            }
            else if (currentCharacter == mDelimiter) {
                if (fieldState == FieldState::QUOTED) {
                    currentWord += currentCharacter;
					LOG_IF("[" << currentCharacter << "][" + FSSTR(fieldState) << "][" << CWS(currentWord) << "]");
                }
                else {
					record.push_back(currentWord);
                    currentWord = "";
					LOG_IF("[" << currentCharacter << "][" + FSSTR(fieldState) << "][" << CWS(currentWord) << "]");
					fieldState = FieldState::UNQUOTED;
                }
            }
            else if (currentCharacter == mQuotation) {
                if (fieldState == FieldState::QUOTED) {
					LOG_IF("[" << currentCharacter << "][" + FSSTR(fieldState) << "][" << CWS(currentWord) << "]");
					fieldState = FieldState::QUOTE_IN_QUOTE;
                }
                else if (fieldState == FieldState::UNQUOTED) {
					LOG_IF("[" << currentCharacter << "][" + FSSTR(fieldState) << "][" << CWS(currentWord) << "]");
					fieldState = FieldState::QUOTED;
                }
                else {
                    currentWord += currentCharacter;
					LOG_IF("[" << currentCharacter << "][" + FSSTR(fieldState) << "][" << CWS(currentWord) << "]");
					fieldState = FieldState::QUOTE_IN_QUOTE;
                }
            }
            else {
				currentWord += currentCharacter;
				LOG_IF("[" << currentCharacter << "][" + FSSTR(fieldState) << "][" << CWS(currentWord) << "]");
                if (fieldState == FieldState::QUOTE_IN_QUOTE) {
                    fieldState = FieldState::QUOTED;
                }
            }
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
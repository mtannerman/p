#include "../../csvparser/inc/CSVParser.h"
#include <stdexcept>
#include <iostream>
#include <deque>

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
			return "u";
		case FieldState::QUOTED:
			return "q";
		case FieldState::QUOTE_IN_QUOTE:
			return "Q";
	}
	return "INVALID";
}

std::string CWS(const std::string& w) {
    std::string ret;
    for (auto c : w) {
        if (c == '\n') {
            ret += "\\n";
        }
		else if (c == '\r') {
			ret += "\\r";
		}
        else {
            ret += c;
        }
    }
    return ret;
}

std::string CSSS(const std::istream& is) {
	switch (is.rdstate()) {
		case std::ios_base::goodbit: return "g";
		case std::ios_base::badbit:  return "b";
		case std::ios_base::failbit: return "f";
		case std::ios_base::eofbit:  return "e";
		case std::ios_base::app:     return "app";
		case std::ios_base::basefield:     return "bf";
		case std::ios_base::binary:     return "binary";
		case std::ios_base::boolalpha:     return "boolalpha";
		case std::ios_base::dec:     return "dec";
		default: return "u";
	}
}

bool CSVParser::readRecord(std::vector<std::string>& record, const bool doLogging)
{
    record.clear();
    // auto state = std::ios_base::goodbit;
    //const std::istream::sentry streamSentry(mStream, true);
    const bool isStreamOk = bool(std::istream::sentry(mStream, true));
	if (!isStreamOk) LOG_IF(mStream.rdstate() << "  goodbit: " << std::ios_base::goodbit << "  badbit: " << std::ios_base::badbit << "  failbit: " << std::ios_base::failbit << "  eofbit: " << std::ios_base::eofbit);
    if (isStreamOk) {
        std::string currentWord;
        auto fieldState = FieldState::UNQUOTED;
		//std::deque<char> charStack;
		//charStack.push_back(GetCurrentCharacter());
        for (auto currentCharacter = GetCurrentCharacter(); ; currentCharacter = GetNextCharacter()) {
            if (std::char_traits<char>::eq_int_type(currentCharacter, std::char_traits<char>::eof())) {
                //mStream.setstate(std::ios_base::eofbit);
                if (!currentWord.empty()) {
                    record.push_back(currentWord);
					LOG_IF("[eof][" + FSSTR(fieldState) << "][" << CWS(currentWord) << "][" << CSSS(mStream) << "]");
                }
                if (fieldState == FieldState::QUOTED) {
                    throw std::invalid_argument("Quote mismatch.");
                }

				return !record.empty();
            }
            else if (currentCharacter == '\n') {
                if (fieldState == FieldState::QUOTE_IN_QUOTE) {
                    record.push_back(currentWord);
					LOG_IF("[\\n ][" + FSSTR(fieldState) << "][" << CWS(currentWord) << "][" << CSSS(mStream) << "]");
                    GetNextCharacter();
                    return true;
                }
                else if (fieldState == FieldState::QUOTED) {
                    currentWord += currentCharacter;
					LOG_IF("[\\n ][" + FSSTR(fieldState) << "][" << CWS(currentWord) << "][" << CSSS(mStream) << "]");
                }
                else {
					record.push_back(currentWord);
                    currentWord = "";
                    LOG_IF("[\\n ][" + FSSTR(fieldState) << "][" << CWS(currentWord) << "][" << CSSS(mStream) << "]");
                    GetNextCharacter();
                    return true;
                }
            }
            else if (currentCharacter == '\r') {
                 if (fieldState == FieldState::QUOTE_IN_QUOTE || fieldState == FieldState::UNQUOTED) {
					 record.push_back(currentWord);
					 currentWord = "";
					 const auto nextCharacter = GetNextCharacter();
					 if (nextCharacter == '\n') {
						 GetNextCharacter();
					 }
					 else {
						 mStream.putback(nextCharacter);
					 }
					 return true;
                }
                else {
					const auto nextCharacter = GetNextCharacter();
					if (nextCharacter == '\n') {
						currentWord += '\n';
					}
					else {
						currentWord += '\r';
						mStream.putback(nextCharacter);
					}
					LOG_IF("[\\r ][" + FSSTR(fieldState) << "][" << CWS(currentWord) << "][" << CSSS(mStream) << "]");
                }
            }
            else if (currentCharacter == mDelimiter) {
                if (fieldState == FieldState::QUOTED) {
                    currentWord += currentCharacter;
					LOG_IF("[ " << currentCharacter << " ][" + FSSTR(fieldState) << "][" << CWS(currentWord) << "][" << CSSS(mStream) << "]");
                }
                else {
					record.push_back(currentWord);
                    currentWord = "";
					LOG_IF("[ " << currentCharacter << " ][" + FSSTR(fieldState) << "][" << CWS(currentWord) << "][" << CSSS(mStream) << "]");
					fieldState = FieldState::UNQUOTED;
                }
            }
            else if (currentCharacter == mQuotation) {
                if (fieldState == FieldState::QUOTED) {
					LOG_IF("[ " << currentCharacter << " ][" + FSSTR(fieldState) << "][" << CWS(currentWord) << "][" << CSSS(mStream) << "]");
					fieldState = FieldState::QUOTE_IN_QUOTE;
                }
                else if (fieldState == FieldState::UNQUOTED) {
					LOG_IF("[ " << currentCharacter << " ][" + FSSTR(fieldState) << "][" << CWS(currentWord) << "][" << CSSS(mStream) << "]");
					fieldState = FieldState::QUOTED;
                }
                else {
                    currentWord += currentCharacter;
					LOG_IF("[ " << currentCharacter << " ][" + FSSTR(fieldState) << "][" << CWS(currentWord) << "][" << CSSS(mStream) << "]");
					fieldState = FieldState::QUOTED;
                }
            }
            else {
				currentWord += currentCharacter;
				LOG_IF("[ " << currentCharacter << " ][" + FSSTR(fieldState) << "][" << CWS(currentWord) << "][" << CSSS(mStream) << "]");
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


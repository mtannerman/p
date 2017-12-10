#include "../../csvparser/inc/CSVParser.h"
#include <stdexcept>
#include <iostream>
#include <deque>


CSVParser::CSVParser(std::istream& istream, const char delimiter, const char quotation)
    : mStream(istream)
    , mDelimiter(delimiter)
    , mQuotation(quotation)
{
	if (mDelimiter == '\r' || mDelimiter == '\n') throw std::invalid_argument("Bad delimiter argument.");
	if (mQuotation == '\r' || mQuotation == '\n') throw std::invalid_argument("Bad quotation argument.");
	if (mQuotation == mDelimiter)                 throw std::invalid_argument("Bad argument. Quotation equals delimiter.");
}

// Enumaration class to express the state of quotedness of fields
enum class FieldState { UNQUOTED, QUOTED, QUOTE_IN_QUOTE };

// A finite state machine implementation to read csv records
bool CSVParser::readRecord(std::vector<std::string>& record)
{
    record.clear();
    const bool isStreamOk = bool(std::istream::sentry(mStream, true));
    if (isStreamOk) {
        std::string currentWord;
        auto fieldState = FieldState::UNQUOTED;
        for (auto currentCharacter = GetCurrentCharacter(); ; currentCharacter = GetNextCharacter()) {
            if (std::char_traits<char>::eq_int_type(currentCharacter, std::char_traits<char>::eof())) {
                if (!currentWord.empty()) {
                    record.push_back(currentWord);
                }
                if (fieldState == FieldState::QUOTED) {
                    throw std::invalid_argument("Quote mismatch.");
                }

				return !record.empty();
            }
            else if (currentCharacter == '\n') {
				switch (fieldState) {
					case FieldState::UNQUOTED:
						record.push_back(currentWord);
						currentWord = "";
						GetNextCharacter();
						return true;
					case FieldState::QUOTED:
						currentWord += currentCharacter;
						break;
					case FieldState::QUOTE_IN_QUOTE:
						record.push_back(currentWord);
						GetNextCharacter();
						return true;
				}
            }
            else if (currentCharacter == '\r') {
				switch (fieldState) {
					case FieldState::QUOTED:
						if (GetNextCharacter() == '\n') {
							currentWord += '\n';
						}
						break;
					default:
						record.push_back(currentWord);
						currentWord = "";
						if (GetNextCharacter() == '\n') {
							GetNextCharacter();
						}
						return true;
				}
            }
            else if (currentCharacter == mDelimiter) {
				switch (fieldState) {
					case FieldState::QUOTED:
						currentWord += currentCharacter;
						break;
					default:
						record.push_back(currentWord);
						currentWord = "";
						fieldState = FieldState::UNQUOTED;
						break;
				}
            }

            else if (currentCharacter == mQuotation) {
				switch (fieldState) {
					case FieldState::QUOTED: 
						fieldState = FieldState::QUOTE_IN_QUOTE;
						break;
					case FieldState::UNQUOTED:
						fieldState = FieldState::QUOTED;
						break;
					default:
						currentWord += currentCharacter;
						fieldState = FieldState::QUOTED;
						break;
				}
            }
            else {
				currentWord += currentCharacter;
                if (fieldState == FieldState::QUOTE_IN_QUOTE) {
                    fieldState = FieldState::QUOTED;
                }
            }
        }
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


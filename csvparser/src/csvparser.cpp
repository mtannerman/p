#include "../../csvparser/inc/csvparser.h"

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
			return "UNQUOTED";
		case FieldState::QUOTED:
			return "QUOTED";
		case FieldState::QUOTE_IN_QUOTE:
			return "QUOTE_IN_QUOTE";
	}
	return "INVALID";
}

bool CSVParser::readRecord(std::vector<std::string>& record, std::vector<std::string>& log)
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
						log.push_back(STR("reached eof character. current word: " << currentWord));
                    }
                    return true;
                }
                else if (currentCharacter == '\n') {
                    if (fieldState == FieldState::QUOTE_IN_QUOTE) {
                        currentWord += currentCharacter;
						log.push_back(STR("reached newline n character in quoteinquote current word: " << currentWord));
                    }
                    else {
						record.push_back(currentWord);
                        currentWord = "";
                        GetNextCharacter();
						log.push_back(STR("reached newline n character in not quotinquote." << "current word : " << currentWord <<  " state: " << FSSTR(fieldState)));
                        return true;
                    }
                }
                else if (currentCharacter == '\r') {
                     if (fieldState == FieldState::QUOTE_IN_QUOTE) {
						 currentWord += '\n';
						 log.push_back(STR("reached newline r character in quoteinquote. current word: " << currentWord));
                    }
                    else {
                        record.push_back(currentWord);
                        currentWord = "";
						log.push_back(STR("reached newline r character in not quotinquote. current word: " << currentWord << " state: " << FSSTR(fieldState)));
                        const auto nextCharacter = GetNextCharacter();
                        if (nextCharacter == '\n') {
							log.push_back(STR("next is n"));
                            GetNextCharacter();
                        }
                        return true;
                    }
                }
                else if (currentCharacter == mDelimiter) {
                    if (fieldState == FieldState::QUOTED) {
                        currentWord += currentCharacter;
						log.push_back(STR("reached delimiter in state " << FSSTR(fieldState) << " current word: " << currentWord));
                    }
                    else {
						record.push_back(currentWord);
                        currentWord = "";
						log.push_back(STR("reached delimiter in state " << FSSTR(fieldState) << " current word: " << currentWord));
						fieldState = FieldState::UNQUOTED;
                    }
                }
                else if (currentCharacter == mQuotation) {
                    if (fieldState == FieldState::QUOTED) {
						log.push_back(STR("reached quote in state " << FSSTR(fieldState) << " current word: " << currentWord));
						fieldState = FieldState::QUOTE_IN_QUOTE;
                    }
                    else if (fieldState == FieldState::UNQUOTED) {
						log.push_back(STR("reached quote in state " << FSSTR(fieldState) << " current word: " << currentWord));
						fieldState = FieldState::QUOTED;
                    }
                    else {
                        currentWord += currentCharacter;
						log.push_back(STR("reached quote in state " << FSSTR(fieldState) << " current word: " << currentWord));
						fieldState = FieldState::UNQUOTED;
                    }
                }
                else {
					currentWord += currentCharacter;
					log.push_back(STR("normal character "  << currentCharacter << " in state " + FSSTR(fieldState) << " current word: " << currentWord));
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
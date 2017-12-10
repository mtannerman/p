#pragma once

#include <vector>
#include <string>
#include <istream>

// Class to parse CSV files with arbitrary delimiter and quotation characters
// Example of usage:
//      std::istream is = ...
//		std::vector<std::string> record;
//      const char delimiter = ...
//      const char quotation = ...
//      CSVParser p(is, delimiter, quotation);
//      while (p.readRecord(record)) {
//			... process record ...
//		}
//
// Throws std::invalid argument exceptions for bad csv input, 
// or bad choice of delimiter and quotation characters
class CSVParser
{
public:
    CSVParser(std::istream& istream, const char delimiter = ',', const char quotation = '\"');

	// Reads a csv record into the vector<string> reference input. 
	// Return true if a valid record was parsed
	// See comments on implementation details at the implementation 
    bool readRecord(std::vector<std::string>& record);
private:
	// Returns the current character if the input stream
    char GetCurrentCharacter();
	// Advances the stream to the next character, and returns it
    char GetNextCharacter();

    std::istream& mStream;
    const char mDelimiter;
    const char mQuotation;
};
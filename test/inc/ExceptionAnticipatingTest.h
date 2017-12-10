#pragma once

#pragma once
#include <string>
#include <vector>
#include "TestManager.h"

// Class to test the exceptions thrown by CSVParser for a given input.
// mInput -> contains the csv input
// desiredErrorDescription -> contains the desired exception description
// We assume, that CSVParser throws std::invalid_arguments.
// mDelimiter -> contains the csv parser's delimiter character
// mQuotation -> contains the csv parser's quotation character
class ExceptionAnticipatingTest
{
public:
	ExceptionAnticipatingTest(const std::string& input,
		const std::string& desiredErrorDescription,
		const char delimiter = ',',
		const char quotation = '\"');

	// Compares the thrown exception description to the desired exception description
	// Compares the thrown exception type to the desired exception type (std::invalid_argument)
	// Fails if there is no exception thrown
	testutil::TestResult Evaluate() const;
private:
	const char mDelimiter;
	const char mQuotation;
	std::string mInput;
	std::string mDesiredErrorDescription;
};
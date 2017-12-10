#pragma once
#include <string>
#include <vector>
#include "TestManager.h"


// class to test CSVParser's output to a given input.
// mInput -> contains the csv input
// mDesiredOutput -> contains the desired parsed csv matrix
// mDelimiter -> contains the csv parser's delimiter character
// mQuotation -> contains the csv parser's quotation character
class SimpleCSVParserTest
{
public:
	SimpleCSVParserTest(const std::string& input,
		const std::vector<std::vector<std::string>>& desiredOutput,
		const char delimiter = ',',
		const char quotation = '\"');

	// Compares the CSVParser's output with the desired output
	testutil::TestResult Evaluate() const;
private:
	const char mDelimiter;
	const char mQuotation;
	const std::string mInput;
	const std::vector<std::vector<std::string>> mDesiredOutput;
};


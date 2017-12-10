#include "../inc/SimpleParserTest.h" 
#include <fstream>
#include "../../csvparser/inc/CSVParser.h"

bool CompareStringMatrices(
	const std::vector<std::vector<std::string>>& strMx1,
	const std::vector<std::vector<std::string>>& strMx2)
{
	if (strMx1.size() == strMx2.size()) {
		for (size_t i = 0; i < strMx1.size(); ++i) {
			if (strMx1[i].size() == strMx2[i].size()) {
				for (size_t j = 0; j < strMx1[i].size(); ++j) {
					if (strMx1[i][j] != strMx2[i][j]) {
						return false;
					}
				}
			}
			else {
				return false;
			}
		}
	}
	else {
		return false;
	}
	return true;
}

SimpleCSVParserTest::SimpleCSVParserTest(const std::string& input, 
	const std::vector<std::vector<std::string>>& desiredOutput,
	const char delimiter,
	const char quotation)
	: mInput(input)
	, mDesiredOutput(desiredOutput)
	, mDelimiter(delimiter)
	, mQuotation(quotation)
{ }

testutil::TestResult SimpleCSVParserTest::Evaluate() const
{
	std::istringstream is(mInput);
	CSVParser parser(is, mDelimiter, mQuotation);
	std::vector<std::vector<std::string>> output;
	std::vector<std::string> row;
	try {
		while (parser.readRecord(row)) {
			output.push_back(row);
		}

		return testutil::TestResult(CompareStringMatrices(mDesiredOutput, output), "");
	}
	catch (const std::invalid_argument& e) {
		return testutil::TestResult(false, STR("invalid argument exception thrown. what():\n" << e.what()));
	}
	return testutil::TestResult(false, "Unreachable code.");
}
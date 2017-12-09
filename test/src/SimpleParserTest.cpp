#include "../inc/SimpleParserTest.h" 
#include <fstream>
#include "../../csvparser/inc/CSVParser.h"

SimpleParserTest SimpleParserTest::ConstructFromFileName(
	const std::string& fileName,
	const std::vector<std::vector<std::string>>& desiredOutput)
{
	std::ifstream is(fileName);
	if (!is.is_open()) throw std::invalid_argument("Couldn't open test input file.");

	const std::string input(std::istreambuf_iterator<char>(is), {});
	return SimpleParserTest(input, desiredOutput);
}

SimpleParserTest::SimpleParserTest(const std::string& input, const std::vector<std::vector<std::string>>& desiredOutput)
	: mInput(input)
	, mDesiredOutput(desiredOutput)
{ }

testutil::TestResult SimpleParserTest::Evaluate() const
{
	std::istringstream is(mInput);
	CSVParser parser(is);
	std::vector<std::vector<std::string>> output;
	std::vector<std::string> row;
	try {
		while (parser.readRecord(row)) {
			output.push_back(row);
		}

		return testutil::TestResult(testutil::CompareStringMatrices(mDesiredOutput, output), "");
	}
	catch (const std::invalid_argument& e) {
		return testutil::TestResult(false, STR("invalid argument exception thrown. what():\n" << e.what()));
	}
	return testutil::TestResult(false, "Unreachable code.");
}
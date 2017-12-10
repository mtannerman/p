#include "..\inc\ExceptionAnticipatingTest.h"
#include "../../csvparser/inc/CSVParser.h"

ExceptionAnticipatingTest::ExceptionAnticipatingTest(
	const std::string& input,
	const std::string& desiredErrorDescription,
	const char delimiter,
	const char quotation)
	: mInput(input)
	, mDesiredErrorDescription(desiredErrorDescription)
	, mDelimiter(delimiter)
	, mQuotation(quotation)
{ }

testutil::TestResult ExceptionAnticipatingTest::Evaluate() const
{
	std::istringstream is(mInput);
	try {
		CSVParser parser(is, mDelimiter, mQuotation);
		std::vector<std::string> row;
		while (parser.readRecord(row));
	}
	catch (const std::invalid_argument& e) {
		if (std::string(e.what()) == mDesiredErrorDescription) {
			return testutil::TestResult(true, "");
		}
		else {
			return testutil::TestResult(false, STR("invalid argument exception thrown with differen description:\n" << e.what()));
		}
	}
	catch (...) {
		return testutil::TestResult(false, "bad exception type thrown");
	}
	return testutil::TestResult(false, "Unreachable code.");
}

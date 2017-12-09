#include "../inc/tests.h"
#include "../inc/testmanager.h"
#include "../../csvparser/inc/csvparser.h"
#include <stdexcept>
#include <fstream>

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


struct SimpleParserTest
{
    SimpleParserTest(const std::string& input, 
    const std::vector<std::vector<std::string>>& desiredOutput);
    static SimpleParserTest ConstructFromFileName(
        const std::string& fileName, 
        const std::vector<std::vector<std::string>>& desiredOutput);
    tu::TestResult Evaluate() const;

    std::string mInput;
    std::vector<std::vector<std::string>> mDesiredOutput;
};

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

tu::TestResult SimpleParserTest::Evaluate() const
{
    std::istringstream is(mInput);
    CSVParser parser(is);
    std::vector<std::vector<std::string>> output;
	std::vector<std::string> row;
	try {
		while (parser.readRecord(row)) {
			output.push_back(row);
		}
		
		return tu::TestResult(CompareStringMatrices(mDesiredOutput, output), "");
	}
	catch (const std::invalid_argument& e) {
		return tu::TestResult(false, STR("invalid argument exception thrown. what():\n" << e.what()));
	}
	return tu::TestResult(false, "Unreachable code.");
}


void RunTests()
{
	

    ADD_TEST("quote in quote 1", {
        return SimpleParserTest("\"aaa\",\"b\"\"bb\",\"222, 333\"\n",
            {{{"aaa"},{"b\"bb"},{"222, 333"}}}).Evaluate();
    }); 

	ADD_TEST("newline in quotes", {
		return SimpleParserTest("\"aa\naa\"\n",
			{{{ "aa\naa" }}}).Evaluate();
	});

	ADD_TEST("newline in quotes without ending newline", {
		return SimpleParserTest("\"aa\naa\"",
		{{{"aa\naa"}}}).Evaluate();
	});

	ADD_TEST("newline in quotes with CRLF", {
		return SimpleParserTest("\"aa\naa\"\r\n",
		{{{"aa\naa"}}}).Evaluate();
	});

	ADD_TEST("CRLF in quotes", {
		return SimpleParserTest("\"aa\r\naa\"",
		{{{"aa\naa"}}}).Evaluate();
	});

	ADD_TEST("CRLF in quotes with CR ending", {
		return SimpleParserTest("\"aa\r\naa\"\r",
		{{{"aa\naa"}}}).Evaluate();
	});

	ADD_TEST("empty input", {
		return SimpleParserTest("",
		std::vector<std::vector<std::string>>()).Evaluate();
	});

	ADD_TEST("empty field", {
		return SimpleParserTest("a,b,,d",
		{{"a","b","","d"}}).Evaluate();
	});

	ADD_TEST("mixed 1", {
		return SimpleParserTest("a,b,,d\naa\r\nb,\"a\rb\"",
		{{"a","b","","d"}, {"aa"}, {"b", "a\rb"}}).Evaluate();
	});

    tu::TestManager::GetInstance().RunAll();   
}
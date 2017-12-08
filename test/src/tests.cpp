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


struct ParserTest
{
    ParserTest(const std::string& input, 
    const std::vector<std::vector<std::string>>& desiredOutput);
    static ParserTest ConstructFromFileName(
        const std::string& fileName, 
        const std::vector<std::vector<std::string>>& desiredOutput);
    bool Evaluate() const;

    std::string mInput;
    std::vector<std::vector<std::string>> mDesiredOutput;
};

ParserTest ParserTest::ConstructFromFileName(
    const std::string& fileName, 
    const std::vector<std::vector<std::string>>& desiredOutput)
{
    std::ifstream is(fileName);
    if (!is.is_open()) throw std::invalid_argument("Couldn't open test input file.");

    const std::string input(std::istreambuf_iterator<char>(is), {});
    return ParserTest(input, desiredOutput);
}

ParserTest::ParserTest(const std::string& input, const std::vector<std::vector<std::string>>& desiredOutput)
    : mInput(input)
    , mDesiredOutput(desiredOutput)
{ }

bool ParserTest::Evaluate() const
{
    std::istringstream is(mInput);
    CSVParser parser(is);
    std::vector<std::vector<std::string>> output;
    output.push_back(std::vector<std::string>());
    while (parser.readRecord(output.back())) {

    }

    return CompareStringMatrices(mDesiredOutput, output);
}


void RunTests()
{
    ADD_TEST("oneliner1", {
        return ParserTest::ConstructFromFileName(
            "input/5.csv", 
            std::vector<std::vector<std::string>>{
                {"aaa","b\"\"bb","222, 333"}
            }).Evaluate();
    }); 

    tu::TestManager::GetInstance().RunAll();   
}
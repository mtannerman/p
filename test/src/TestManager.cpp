#include "../../test/inc/TestManager.h"
#include <algorithm>

namespace testutil
{

TestManager& TestManager::GetInstance()
{
    static TestManager instance;
    return instance;    
}

void TestManager::AddTest(const std::string id, std::function<TestResult()> testFunction)
{
    mTests.push_back(Test{id, testFunction});
}

size_t CountLongestIdLength(const std::vector<Test>& tests)
{
    size_t maxLength = tests.front().id.size();
    for (size_t i = 1; i < tests.size(); ++i) {
        maxLength = std::max(tests[i].id.size(), maxLength);
    }

    return maxLength;
}

void PrintTestResult(const std::string& testId, const TestResult& result, const size_t longestIdLength)
{
    const size_t nSpacesBefore = (longestIdLength - testId.size()) / 2;
    const size_t nSpacesAfter = longestIdLength - testId.size() - nSpacesBefore;
    std::cout << '[';
    for (int i = 0; i < nSpacesBefore; ++i) {
        std::cout << ' ';
    }

	std::cout << testId;

    for (int i = 0; i < nSpacesAfter; ++i) {
        std::cout << ' ';
    }

	if (result.mPassed) {
		std::cout << "] passed" << std::endl;
	}
	else {
		std::cout << "] FAILED: " << result.mDescription << std::endl;
	}
}

void TestManager::RunAll() const
{
    if (mTests.empty()) return;

    const auto longestIdLength = CountLongestIdLength(mTests);
    size_t nPassed = 0;
    for (const auto& test : mTests) {
		const auto result = test.resultGetter();
        PrintTestResult(test.id, result, longestIdLength);
        nPassed += size_t(result.mPassed);
    }
    std::cout << nPassed << "/" << mTests.size() << " tests passed" << std::endl;
    std::cout << "=============================================================" << std::endl;
}    
}
#include "../../test/inc/testmanager.h"

namespace tu
{

TestManager& TestManager::GetInstance()
{
    static TestManager instance;
    return instance;    
}

void TestManager::AddTest(const std::string id, std::function<bool()> testFunction)
{
    mTests.push_back(IdTestPair{id, testFunction});
}

size_t CountLongestIdLength(const std::vector<IdTestPair>& idTestPairs)
{
    size_t maxLength = idTestPairs.front().id.size();
    for (size_t i = 1; i < idTestPairs.size(); ++i) {
        maxLength = std::max(idTestPairs[i].id.size(), maxLength);
    }

    return maxLength;
}

void PrintTestResult(const std::string& id, const bool passed, const size_t longestIdLength)
{
    const size_t nSpacesBefore = (longestIdLength - id.size()) / 2;
    const size_t nSpacesAfter = longestIdLength - id.size() - nSpacesBefore;
    std::cout << '[';
    for (int i = 0; i < nSpacesBefore; ++i) {
        std::cout << ' ';
    }

    std::cout << id;

    for (int i = 0; i < nSpacesAfter; ++i) {
        std::cout << ' ';
    }
    std::cout << "] " << (passed ? "passed" : "FAILED") << std::endl;
}

void TestManager::RunAll() const
{
    if (mTests.empty()) return;

    const auto longestIdLength = CountLongestIdLength(mTests);
    size_t nPassed = 0;
    for (const auto& test : mTests) {
        const bool passed = test.function();
        PrintTestResult(test.id, passed, longestIdLength);
        nPassed += size_t(passed);
    }
    std::cout << nPassed << "/" << mTests.size() << " tests passed" << std::endl;
    std::cout << "=============================================================" << std::endl;
}    
}
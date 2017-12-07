#include "../../testutil/inc/testutil.h"

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

void TestManager::RunAll() const
{
    for (const auto& idTest : mTests) {
        std::cout << idTest.id << (idTest.function() ? " passed" : " failed") << std::endl;
    }
}    
}
#pragma once

#include <vector>
#include <string>
#include <functional>
#include <iostream>

#define ADD_TEST(testIdString, testLambdaBody) ::tu::TestManager::GetInstance().AddTest(testIdString, [](){ testLambdaBody })

namespace tu
{

struct IdTestPair
{
    std::string id;
    std::function<bool()> function;
};

class TestManager
{
public:
    static TestManager& GetInstance();
    void AddTest(const std::string id, std::function<bool()> testFunction);
    void RunAll() const;
private:
    
    TestManager() = default;
    std::vector<IdTestPair> mTests;
};



}
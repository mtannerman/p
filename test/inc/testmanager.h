#pragma once

#include <vector>
#include <string>
#include <functional>
#include <iostream>

#define ADD_TEST(testIdString, testLambdaBody) ::tu::TestManager::GetInstance().AddTest(testIdString, [](){ testLambdaBody })

namespace tu
{

struct TestResult
{
	TestResult(const bool passed, const std::string description)
		: mPassed(passed)
		, mDescription(description)
	{ }
	const bool mPassed;
	const std::string mDescription;
};

struct Test
{
    std::string id;
    std::function<TestResult()> resultGetter;
};

class TestManager
{
public:
    static TestManager& GetInstance();
    void AddTest(const std::string id, std::function<TestResult()> testFunction);
    void RunAll() const;
private:
    
    TestManager() = default;
    std::vector<Test> mTests;
};



}
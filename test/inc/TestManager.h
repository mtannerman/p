#pragma once

#include <vector>
#include <string>
#include <functional>
#include <iostream>
#include <sstream>

// Macro to add unit test instances to the TestManager singleton object 
#define ADD_TEST(testIdString, testLambdaBody) ::testutil::TestManager::GetInstance().AddTest(testIdString, [](){ testLambdaBody })
// Helpful macro for building strings with insertion operators
#define STR(msg) (((::std::ostringstream*)&( ::std::ostringstream() << "" << msg))->str())

namespace testutil
{

// Utility struct for TestManager
// Stores information about test run results
struct TestResult
{
	TestResult(const bool passed, const std::string description)
		: mPassed(passed)
		, mDescription(description)
	{ }
	const bool mPassed;
	const std::string mDescription;
};

// Utility struct for TestManager
// Stores the string id, and test function 
struct Test
{
    std::string id;
    std::function<TestResult()> resultGetter;
};


// Singleton object to collect, and evaluate simple unit tests during runtime
// Each test should have a unique string id. The test functions are stored
// in std::function objects, which should have no input, and return a TestResult instance
class TestManager
{
public:
	// Create and get the TestManager singleton instance
    static TestManager& GetInstance();
	// Add test to the test collection mTests
    void AddTest(const std::string id, std::function<TestResult()> testFunction);
	// Run all the stored unit tests
    void RunAll() const;
private:
    
    TestManager() = default;
    std::vector<Test> mTests;
};



}
#pragma once
#include <string>
#include <vector>
#include "TestManager.h"

struct SimpleParserTest
{
	SimpleParserTest(const std::string& input,
		const std::vector<std::vector<std::string>>& desiredOutput);
	static SimpleParserTest ConstructFromFileName(
		const std::string& fileName,
		const std::vector<std::vector<std::string>>& desiredOutput);
	testutil::TestResult Evaluate() const;

	std::string mInput;
	std::vector<std::vector<std::string>> mDesiredOutput;
};


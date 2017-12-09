#pragma once

#pragma once
#include <string>
#include <vector>
#include "TestManager.h"


struct ExceptionAnticipatingTest
{
	ExceptionAnticipatingTest(const std::string& input,
		const std::string& desiredErrorDescription);
	testutil::TestResult Evaluate() const;

	std::string mInput;
	std::string mDesiredErrorDescription;
};
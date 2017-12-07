#pragma once

#include <vector>
#include <string>
#include <functional>
#include <iostream>

namespace tu
{
class TestManager
{
public:
    TestManager& GetInstance();
    void AddTest(const std::string id, std::function<bool()> testFunction);
    void RunAll() const;
private:
    struct IdTestPair
    {
        std::string id;
        std::function<bool()> function;
    };
    TestManager() = default;
    std::vector<IdTestPair> mTests;
};



}
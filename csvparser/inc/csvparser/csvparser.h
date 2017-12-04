#pragma once

#include <vector>
#include <string>
#include <memory>
#include <istream>

class CSVParser
{
public:
    CSVParser(std::istream& istream, const char delimiter = ',', const char quotationMark = '\"');
    bool readRecord(std::vector<std::string>& record);
private:

};
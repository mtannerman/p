#include <iostream>
#include "testutil/testutil.h"
#include "csvparser/csvparser.h"
#include <fstream>

void print_contents(const std::string& path) 
{
    std::ifstream in(path);
    std::string line;
    while (std::getline(in, line)) {
        std::cout << line << std::endl;
    }
}

int main()
{
    const std::string inputFilePath = "../input/4.csv";
    print_contents(inputFilePath);
    std::cout << std::endl << std::endl;
    std::ifstream in(inputFilePath);
    CSVParser parser(in);
    std::vector<std::string> record;
    while (parser.readRecord(record)) {
        for (const auto& word : record) {
            std::cout << word << "||";
        }
        std::cout << std::endl;
    }

    std::cout << "finito" << std::endl;
    return 0;
}
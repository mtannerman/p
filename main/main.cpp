#include <iostream>
#include "../csvparser/inc/csvparser.h"
#include "../testutil/inc/testutil.h"
#include <fstream>
#include <sstream>



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
    const std::string inputFilePath = "input/5.csv";
    // const std::string inputFilePath = "../../input/5.csv";
    print_contents(inputFilePath);
    std::cout << std::endl << std::endl;
    std::ifstream in(inputFilePath);
    CSVParser parser(in);
	std::vector<std::string> record;
    std::stringstream ss;
    while (parser.readRecord(record, true)) {
		std::cout << "new record: " << record.size() << std::endl;
        for (auto it = record.begin(); it != std::prev(record.end()); ++it) {
            ss << *it << "||";
        }
		ss << *std::prev(record.end()) << std::endl;
    }

    std::cout << "\nOutput:\n" << ss.str() << std::endl;
    std::cout << "finito" << std::endl;
	// std::cin.get();
    return 0;
}
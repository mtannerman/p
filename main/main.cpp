#include <iostream>
#include "../csvparser/inc/csvparser.h"
#include "../testutil/inc/testutil.h"
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
    const std::string inputFilePath = "../../input/5.csv";
    print_contents(inputFilePath);
    std::cout << std::endl << std::endl;
    std::ifstream in(inputFilePath);
    CSVParser parser(in);
	std::vector<std::string> record;
	std::vector<std::vector<std::string>> logs;
	logs.push_back(std::vector<std::string>());
    while (parser.readRecord(record, logs.back())) {
		//std::cout << '.' << std::endl;
        for (const auto& word : record) {
            std::cout << word << "||";
        }
		logs.push_back(std::vector<std::string>());
		std::cout << std::endl;
    }

	std::cout << "\n\nLOGS:";
	for (const auto& log : logs) {
		for (const auto& e : log) {
			std::cout << e << std::endl;
		}
		std::cout << "..................................";
	}

    std::cout << "finito" << std::endl;
	std::cin.get();
    return 0;
}
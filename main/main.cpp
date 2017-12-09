#include <iostream>
#include "../csvparser/inc/CSVParser.h"
#include "../test/inc/tests.h"
#include <fstream>

void print_contents(const std::string& path) 
{
    std::ifstream in(path);
    std::string line;
    while (std::getline(in, line)) {
        std::cout << line << std::endl;
    }
}

void print_input_string(const std::string& i)
{
	std::cout << "Input:\n";
	for (auto c : i) {
		switch (c) {
			case '\r': std::cout << "\\r"; break;
			case '\n': std::cout << "\\n"; break;
			default:   std::cout << c; break;
		}
	}
	std::cout << "\n\n";
}

int main()
{
    RunTests();
    //const std::string inputFilePath = "input/5.csv";
    //const std::string inputFilePath = "../../input/6.csv";
    //print_contents(inputFilePath);
    //std::cout << std::endl << std::endl;
    //std::ifstream in(inputFilePath);
	const std::string inputstring = "\" Simple Hill \"\"\"\"6\"";
	print_input_string(inputstring);
	std::istringstream in(inputstring);
    CSVParser parser(in);
	std::vector<std::string> record;
    std::stringstream ss;
    while (parser.readRecord(record, true)) {
		// std::cout << "new record: " << record.size() << std::endl;
        for (auto it = record.begin(); it != std::prev(record.end()); ++it) {
            ss << *it << "||";
        }
		ss << *std::prev(record.end());
    }

    std::cout << "\nOutput:\n" << "[" << ss.str() << "]" << std::endl;
    std::cout << "finito" << std::endl;
	std::cin.get();
    return 0;
}
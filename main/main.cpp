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
	/*
	const std::string inputstring = "a,a,a,a\r\nb,b,b,b\nc,c,c,c\rd,d,d,d\r\n";
	print_input_string(inputstring);
	std::istringstream in(inputstring);
    CSVParser parser(in);
	std::vector<std::string> record;
	std::vector<std::stringstream> outputStreams;
    while (parser.readRecord(record, true)) {
		outputStreams.push_back(std::stringstream());
		// std::cout << "new record: " << record.size() << std::endl;
        for (auto it = record.begin(); it != std::prev(record.end()); ++it) {
            outputStreams.back() << *it << "||";
        }
		outputStreams.back() << *std::prev(record.end());
    }

	for (size_t i = 0; i < outputStreams.size(); ++i) {
		std::cout << "\nOutput line " << i << ":\n" << "[" << outputStreams[i].str() << "]" << std::endl;
	}
    std::cout << "finito" << std::endl;
	*/
	std::cin.get();
    return 0;
}
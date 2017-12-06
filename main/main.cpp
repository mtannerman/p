#include <iostream>
#include "testutil/testutil.h"
#include "csvparser/csvparser.h"
#include <fstream>

int main()
{
    std::ifstream in("../input/3.csv");
    CSVParser parser(in);
    std::vector<std::string> records;
    

    std::cout << "finito" << std::endl;
    return 0;
}
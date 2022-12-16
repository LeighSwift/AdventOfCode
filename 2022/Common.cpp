#include "Common.h"

#include <fstream>
#include <sstream>

std::string AoC::FileSystem::ReadAllText(const char *filename)
{
    std::string result;
    std::ifstream inputFile(filename);
    if (inputFile.is_open())
    {
        std::stringstream buffer;
        buffer << inputFile.rdbuf();
        result = buffer.str();
    }
    return result;
}

std::vector<std::string> AoC::FileSystem::ReadAllLines(const char *filename)
{
    std::vector<std::string> result;
    std::ifstream inputFile(filename);
    if (inputFile.is_open())
    {
        std::string line;
        while (std::getline(inputFile, line))
        {
            result.push_back(std::move(line));
        }
    }
    return result;
}

AoC::DayFunc Days[25];
AoC::DaySetter::DaySetter(int day, DayFunc &&func)
{
    Days[day-1] = std::move(func);
}
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>

// https://adventofcode.com/2015

namespace AoC
{
    class FileSystem
    {
    public:
        static std::string ReadAllText(const char *filename)
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
        static std::vector<std::string> ReadAllLines(const char *filename)
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
    };
}

void day01()
{
    std::string inputData = AoC::FileSystem::ReadAllText("input01.txt");
    int floor = 0;
    int firstBasementEntryPos = 0;
    for (size_t i = 0; i < inputData.length(); i++)
    {
        char c = inputData[i];
        switch (c)
        {
        case '(':
            ++floor;
            break;
        case ')':
            --floor;
            break;

        default:
            break;
        }
        if (floor == -1 && firstBasementEntryPos == 0)
        {
            firstBasementEntryPos = i + 1;
        }
    }
    std::cout << "AoC: Day 01: Final floor:     " << floor << std::endl;
    std::cout << "AoC: Day 01: Enters basement: " << firstBasementEntryPos << std::endl;
    std::cout << std::endl;
}

void day02()
{
    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input02.txt");
    int totalWrappingSqFt = 0;
    int totalRibbonFeet = 0;
    for (size_t i = 0; i < inputData.size(); i++)
    {
        std::string &line = inputData[i];
        std::istringstream lineStream(line);
        int l, w, h;
        char x;
        lineStream >> l >> x >> w >> x >> h;
        totalWrappingSqFt += (2 * l * w) + (2 * w * h) + (2 * h * l);
        totalWrappingSqFt += std::min(std::min(l * w, w * h), h * l);

        std::array<int, 3> sides = {l, w, h};
        std::sort(sides.begin(), sides.end());

        totalRibbonFeet += l * w * h;
        totalRibbonFeet += sides[0];
        totalRibbonFeet += sides[0];
        totalRibbonFeet += sides[1];
        totalRibbonFeet += sides[1];
    }
    std::cout << "AoC: Day 02: Total wrapping sqft: " << totalWrappingSqFt << std::endl;
    std::cout << "AoC: Day 02: Total ribbon feet:   " << totalRibbonFeet << std::endl;
    std::cout << std::endl;
}

int main()
{
    day01();
    day02();
}

#include "Common.h"

void Day03()
{
    auto getPriority = [&](char c)
    {
        if (c <= 'Z')
        {
            return (c - 'A') + 27;
        }
        else
        {
            return (c - 'a') + 1;
        }
    };
    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("Data/input03.txt");
    int totalPriorityP1 = 0;
    int totalPriorityP2 = 0;
    for (size_t i = 0; i < inputData.size(); i++)
    {
        // Part 1
        {
            std::string &line = inputData[i];
            const size_t compartmentSize = line.length() / 2;
            std::array<char, 52> table{0};
            std::for_each(line.begin(), line.end() - compartmentSize, [&](char c)
                          { table[getPriority(c) - 1] = 1; });
            totalPriorityP1 += getPriority(*std::find_if(line.begin() + compartmentSize, line.end(), [&](char c)
                                                         { return table[getPriority(c) - 1] == 1; }));
        }
        // Part 2
        {
            if (i % 3 == 2)
            {
                std::array<char, 52> table{0};
                std::string &line1 = inputData[i - 2];
                std::string &line2 = inputData[i - 1];
                std::string &line3 = inputData[i];
                std::for_each(line1.begin(), line1.end(), [&](char c)
                              { table[getPriority(c) - 1] |= 1; });
                std::for_each(line2.begin(), line2.end(), [&](char c)
                              { table[getPriority(c) - 1] |= 2; });
                totalPriorityP2 += getPriority(*std::find_if(line3.begin(), line3.end(), [&](char c)
                                                             { return table[getPriority(c) - 1] == 3; }));
            }
        }
    }

    std::cout << "AoC: Day 03: Total priority P1: " << totalPriorityP1 << std::endl;
    std::cout << "AoC: Day 03: Total priority P2: " << totalPriorityP2 << std::endl;
    std::cout << std::endl;
}

AoC::DaySetter DaySetter03(3, Day03);
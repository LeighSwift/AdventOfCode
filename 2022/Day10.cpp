#include "Common.h"

void Day10()
{
    std::vector<std::string> display;
    int xReg = 1;
    int cycle = 0;
    int totalSignal = 0;

    auto clock = [&](int i, int nX)
    {
        const int pixel = i % 40;
        if (pixel == 0)
        {
            display.push_back("");
        }
        display.back() += abs(xReg - pixel) <= 1 ? '#' : ' ';
        if (pixel == 19)
        {
            totalSignal += (i + 1) * xReg;
        }
        xReg = nX;
    };

    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("Data/input10.txt");
    for (size_t i = 0; i < inputData.size(); i++)
    {
        std::istringstream lineStream(inputData[i]);
        std::string op;
        lineStream >> op;
        if (op == "noop")
        {
            clock(cycle++, xReg);
        }
        else if (op == "addx")
        {
            int val;
            lineStream >> val;
            clock(cycle++, xReg);
            clock(cycle++, xReg + val);
        }
    }

    std::cout << "AoC: Day 10: Part 1:  " << totalSignal << std::endl;
    std::cout << "AoC: Day 10: Part 2:" << std::endl;
    for (const std::string &line : display)
    {
        std::cout << line << std::endl;
    }
    std::cout << std::endl;
}

AoC::DaySetter DaySetter10(10, Day10);
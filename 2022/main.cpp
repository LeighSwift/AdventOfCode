#include "Common.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <numeric>
#include <algorithm>
#include <map>

// https://adventofcode.com/2022

void day03()
{
}

void day04()
{
}

void day05()
{
}

void day06()
{
}

void day07()
{
}

void day08()
{
}

void day09()
{
}

void day10()
{
}

void day11()
{
}

void day12()
{
}

void day13()
{
}

void day14()
{
    // std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("Data/input14.txt");
    // for (size_t i = 0; i < inputData.size(); i++)
    // {
    //     std::string &line = inputData[i];
    // }
    // std::cout << "AoC: Day 14: Part 1:  " << todo << std::endl;
    // std::cout << "AoC: Day 14: Part 2:  " << todo << std::endl;
    // std::cout << std::endl;
}

extern AoC::DayFunc Days[];
int main(int argc, char *argv[])
{
    auto runDay = [](int i)
    {
        const AoC::DayFunc &day = Days[i-1];
        if (day)
        {
            day();
        }
    };

    if (argc > 1)
    {
        char* dayToRun = argv[1];
        runDay(atoi(dayToRun));
    }
    else
    {
        for (size_t i = 1; i <= 25; i++)
        {
            runDay(i);
        }
    }
}

#pragma once

#include <array>
#include <chrono>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <thread>
#include <vector>

namespace AoC
{
    class FileSystem
    {
    public:
        static std::string ReadAllText(const char *filename);
        static std::vector<std::string> ReadAllLines(const char *filename);
    };

    typedef std::function<void()> DayFunc;
    struct DaySetter
    {
        public:
            DaySetter(int, DayFunc &&);
    };
}
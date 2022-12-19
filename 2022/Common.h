#pragma once

#include <array>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
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
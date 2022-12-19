#pragma once

#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <map>

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
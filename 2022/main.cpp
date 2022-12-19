#include "Common.h"

// https://adventofcode.com/2022

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

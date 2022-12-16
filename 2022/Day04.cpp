#include "Common.h"

void Day04()
{
    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("Data/input04.txt");
    int numFullOverlaps = 0;
    int numOverlaps = 0;
    for (size_t i = 0; i < inputData.size(); i++)
    {
        std::istringstream lineStream(inputData[i]);
        int a, b, c, d;
        char dummy;
        lineStream >> a >> dummy >> b >> dummy >> c >> dummy >> d;
        // ....a----b....
        // .....c--d.....
        if (a <= c && b >= d)
        {
            ++numFullOverlaps;
            ++numOverlaps;
        }
        // .....a--b.....
        // ....c----d....
        else if (c <= a && d >= b)
        {
            ++numFullOverlaps;
            ++numOverlaps;
        }
        // ..a---b....
        // ....c---d..
        else if (a <= c && b >= c)
        {
            ++numOverlaps;
        }
        // ....a---b..
        // ..c---d....
        else if (a <= d && b >= d)
        {
            ++numOverlaps;
        }
    }
    std::cout << "AoC: Day 04: Num fully overlapped sections:  " << numFullOverlaps << std::endl;
    std::cout << "AoC: Day 04: Num overlapped sections: " << numOverlaps << std::endl;
    std::cout << std::endl;
}

AoC::DaySetter DaySetter04(4, Day04);
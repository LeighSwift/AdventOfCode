#include "Common.h"

void Day12()
{
    struct Loc
    {
    public:
        int x;
        int y;
        char terrain;
        int stepsAway = std::numeric_limits<int>::max();
    };
    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("Data/input12.txt");
    const int numCols = inputData[0].length();
    while (inputData.back().length() < numCols)
    {
        inputData.pop_back();
    }
    const int numRows = inputData.size();
    Loc *map = new Loc[numRows * numCols];
    int startX, startY;
    int endX, endY;
    auto getLoc = [&](int x, int y) -> Loc &
    { return map[x + (y * numCols)]; };
    std::vector<Loc *> lowestPoints;
    for (int y = 0; y < inputData.size(); y++)
    {
        std::string &line = inputData[y];
        for (int x = 0; x < line.length(); x++)
        {
            Loc &loc = getLoc(x, y) = Loc{x, y, line[x]};
            if (loc.terrain == 'S')
            {
                startX = x;
                startY = y;
                loc.terrain = 'a';
            }
            else if (loc.terrain == 'E')
            {
                endX = x;
                endY = y;
                loc.terrain = 'z';
            }
            if (loc.terrain == 'a')
            {
                lowestPoints.push_back(&loc);
            }
        }
    }
    std::vector<Loc *> processQueue;
    processQueue.push_back(&getLoc(endX, endY));
    processQueue.back()->stepsAway = 0;
    auto step = [&](Loc &to, Loc &from)
    {
        // Should no longer do this for part 2
        // if (startLoc->stepsAway <= from.stepsAway + 1)
        //{
        //    return;
        //}
        if (to.stepsAway <= from.stepsAway + 1)
        {
            return;
        }
        if (to.terrain < (from.terrain - 1))
        {
            return;
        }
        to.stepsAway = from.stepsAway + 1;
        processQueue.push_back(&to);
    };
    auto process = [&](Loc &loc)
    {
        if (loc.x < numCols)
        {
            step(getLoc(loc.x + 1, loc.y), loc);
        }
        if (loc.x > 0)
        {
            step(getLoc(loc.x - 1, loc.y), loc);
        }
        if (loc.y < numRows)
        {
            step(getLoc(loc.x, loc.y + 1), loc);
        }
        if (loc.y > 0)
        {
            step(getLoc(loc.x, loc.y - 1), loc);
        }
    };
    for (size_t i = 0; i < processQueue.size(); i++)
    {
        process(*processQueue[i]);
    }
    auto cmp = [&](const Loc *a, const Loc *b)
    {
        return a->stepsAway < b->stepsAway;
    };
    std::sort(lowestPoints.begin(), lowestPoints.end(), cmp);

    std::cout << "AoC: Day 12: Part 1:  " << getLoc(startX, startY).stepsAway << std::endl;
    std::cout << "AoC: Day 12: Part 2:  " << lowestPoints[0]->stepsAway << std::endl;
    std::cout << std::endl;
}

AoC::DaySetter DaySetter12(12, Day12);
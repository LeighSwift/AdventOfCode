#include "Common.h"

void Day14()
{
    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("Data/input14.txt");
    std::vector<std::vector<std::pair<int, int>>> rockLines;
    int xMin = 500;
    int yMin = 0;
    int xMax = 500;
    int yMax = 0;
    for (size_t i = 0; i < inputData.size(); i++)
    {
        rockLines.emplace_back();
        std::vector<std::pair<int, int>> &points = rockLines.back();
        std::istringstream line(inputData[i]);
        while (!line.eof())
        {
            int x, y;
            line >> x;
            line.ignore(1);
            line >> y;
            line.ignore(4);
            points.emplace_back(x, y);
            xMin = std::min(x, xMin);
            yMin = std::min(y, yMin);
            xMax = std::max(x, xMax);
            yMax = std::max(y, yMax);
        }
    }
    yMax+=2;
    xMin = 490 - yMax;
    xMax = 510 + yMax;
    const int width = (xMax - xMin) + 1;
    const int height = (yMax - yMin) + 1;
    char *map = new char[width * height];
    std::memset(map, '.', width * height * sizeof(char));
    auto getLoc = [&](int x, int y) -> char &
    {
        return map[(x - xMin) + ((y - yMin) * width)];
    };
    for (size_t i = 0; i < rockLines.size(); i++)
    {
        for (size_t j = 1; j < rockLines[i].size(); j++)
        {
            const std::pair<int, int> &from = rockLines[i][j - 1];
            const std::pair<int, int> &to = rockLines[i][j];
            // only works to loop this way for horiz/vert lines
            for (int x = std::min(from.first, to.first); x <= std::max(from.first, to.first); x++)
            {
                for (int y = std::min(from.second, to.second); y <= std::max(from.second, to.second); y++)
                {
                    getLoc(x, y) = '#';
                }
            }
        }
    }
    const std::pair<int, int> sandSpawn = {500, 0};
    getLoc(sandSpawn.first, sandSpawn.second) = '+';

    // Draw map for debugging
    std::ofstream drawFile;
    auto draw = [&]()
    {
        drawFile.open("sandMap.txt");
        for (int y = yMin; y <= yMax; y++)
        {
            std::string line = "";
            for (int x = xMin; x <= xMax; x++)
            {
                line += getLoc(x, y);
            }
            drawFile << line << std::endl;
        }
        drawFile.close();
    };
    draw();

    // Drop sand until one hits
    auto dropSand = [&](std::pair<int, int> &sand)
    {
        const char down = getLoc(sand.first, sand.second + 1);
        const char left = getLoc(sand.first - 1, sand.second);
        const char leftDown = getLoc(sand.first - 1, sand.second + 1);
        const char right = getLoc(sand.first + 1, sand.second);
        const char rightDown = getLoc(sand.first + 1, sand.second + 1);
        if (down == '.')
        {
            sand = {sand.first, sand.second + 1};
            return true;
        }
        // else if (left == '.' && leftDown == '.')
        else if (leftDown == '.')
        {
            sand = {sand.first - 1, sand.second + 1};
            return true;
        }
        // else if (right == '.' && rightDown == '.')
        else if (rightDown == '.')
        {
            sand = {sand.first + 1, sand.second + 1};
            return true;
        }
        return false;
    };
    int part1 = 0;
    bool bIntoAbyss = false;
    while (!bIntoAbyss)
    {
        std::pair<int, int> sand = sandSpawn;
        while (dropSand(sand))
        {
        }
        bIntoAbyss = sand.first < xMin || sand.first > xMax || sand.second >= yMax;
        if (!bIntoAbyss)
        {
            getLoc(sand.first, sand.second) = 'o';
            ++part1;
        }
    }
    draw();
    
    for (int x = xMin; x <= xMax; x++)
    {
        getLoc(x, yMax) = '#';
    }
    draw();

    int part2 = part1;
    bool bSpawnBlocked = false;
    while (!bSpawnBlocked)
    {
        std::pair<int, int> sand = sandSpawn;
        while (dropSand(sand))
        {
        }
        bSpawnBlocked = sand == sandSpawn;
        if (!bSpawnBlocked)
        {
            getLoc(sand.first, sand.second) = 'o';
            ++part2;
        }
    }
    getLoc(sandSpawn.first, sandSpawn.second) = 'o';
    ++part2;
    draw();

    std::cout << "AoC: Day 14: Part 1:  " << part1 << std::endl;
    std::cout << "AoC: Day 14: Part 2:  " << part2 << std::endl;
    std::cout << std::endl;
}

AoC::DaySetter DaySetter14(14, Day14);
#include "Common.h"

void Day08()
{
    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("Data/input08.txt");
    const int numRows = inputData.size();
    const int numCols = inputData[0].length();
    char *treeData = new char[numRows * numCols];
    char *treeVisible = new char[numRows * numCols];
    // 8-bits: 0000NESW
    const char northVisible = 8;
    const char eastVisible = 4;
    const char southVisible = 2;
    const char westVisible = 1;
    std::memset(treeVisible, 0, numRows * numCols * sizeof(char));
    auto getTreeHeight = [&](int x, int y) -> char *
    {
        return treeData + (y * numRows) + x;
    };
    auto getTreeVisible = [&](int x, int y) -> char *
    {
        return treeVisible + (y * numRows) + x;
    };
    for (size_t y = 0; y < inputData.size(); y++)
    {
        for (size_t x = 0; x < inputData[y].length(); x++)
        {
            *getTreeHeight(x, y) = inputData[y][x] - '0';
        }
    }
    // Set edges visible
    for (int y = 0; y < numRows; y++)
    {
        *getTreeVisible(0, y) = 0xf;
        *getTreeVisible(numCols - 1, y) = 0xf;
    }
    for (int x = 0; x < numCols; x++)
    {
        *getTreeVisible(x, 0) = 0xf;
        *getTreeVisible(x, numRows - 1) = 0xf;
    }
    // Process north
    for (int x = 1; x < numRows - 1; x++)
    {
        int highest = *getTreeHeight(x, numRows - 1);
        for (int y = numRows - 2; y > 0; y--)
        {
            int treeHeight = *getTreeHeight(x, y);
            if (treeHeight > highest)
            {
                *getTreeVisible(x, y) |= northVisible;
                highest = treeHeight;
            }
            // Can stop if we hit a 9
            if (treeHeight == 9)
            {
                break;
            }
        }
    }
    // Process east
    for (int y = 1; y < numRows - 1; y++)
    {
        int highest = *getTreeHeight(0, y);
        for (int x = 1; x < numCols - 1; x++)
        {
            int treeHeight = *getTreeHeight(x, y);
            if (treeHeight > highest)
            {
                *getTreeVisible(x, y) |= eastVisible;
                highest = treeHeight;
            }
            // Can stop if we hit a 9
            if (treeHeight == 9)
            {
                break;
            }
        }
    }
    // Process south
    for (int x = 1; x < numRows - 1; x++)
    {
        int highest = *getTreeHeight(x, 0);
        for (int y = 1; y < numCols - 1; y++)
        {
            int treeHeight = *getTreeHeight(x, y);
            if (treeHeight > highest)
            {
                *getTreeVisible(x, y) |= southVisible;
                highest = treeHeight;
            }
            // Can stop if we hit a 9
            if (treeHeight == 9)
            {
                break;
            }
        }
    }
    // Process west
    for (int y = 1; y < numRows - 1; y++)
    {
        int highest = *getTreeHeight(numCols - 1, y);
        for (int x = numCols - 2; x > 0; x--)
        {
            int treeHeight = *getTreeHeight(x, y);
            if (treeHeight > highest)
            {
                *getTreeVisible(x, y) |= westVisible;
                highest = treeHeight;
            }
            // Can stop if we hit a 9
            if (treeHeight == 9)
            {
                break;
            }
        }
    }

    // Part 1 - how many visible trees
    int numVisible = 0;
    for (int i = 0; i < numRows * numCols; i++)
    {
        if (treeVisible[i] > 0)
        {
            ++numVisible;
        }
    }

    // Part 2 - Tree score for visibility n * e * s * w
    int highestScore = 0;
    for (int x = 0; x < numCols; x++)
    {
        for (int y = 0; y < numRows; y++)
        {
            int treeHeight = *getTreeHeight(x, y);
            // North
            int countN = 0;
            for (int yy = y - 1; yy >= 0; yy--)
            {
                ++countN;
                int nextTree = *getTreeHeight(x, yy);
                if (nextTree >= treeHeight)
                {
                    break;
                }
            }
            // East
            int countE = 0;
            for (int xx = x + 1; xx < numCols; xx++)
            {
                ++countE;
                int nextTree = *getTreeHeight(xx, y);
                if (nextTree >= treeHeight)
                {
                    break;
                }
            }
            // South
            int countS = 0;
            for (int yy = y + 1; yy < numRows; yy++)
            {
                ++countS;
                int nextTree = *getTreeHeight(x, yy);
                if (nextTree >= treeHeight)
                {
                    break;
                }
            }
            // West
            int countW = 0;
            for (int xx = x - 1; xx >= 0; xx--)
            {
                ++countW;
                int nextTree = *getTreeHeight(xx, y);
                if (nextTree >= treeHeight)
                {
                    break;
                }
            }
            // Total
            int score = countN * countE * countS * countW;
            if (score > highestScore)
            {
                highestScore = score;
            }
        }
    }

    std::cout << "AoC: Day 08: Part 1 total visible:  " << numVisible << std::endl;
    std::cout << "AoC: Day 08: Part 2 highest score:  " << highestScore << std::endl;
    std::cout << std::endl;
    delete[] treeData;
}

AoC::DaySetter DaySetter08(8, Day08);
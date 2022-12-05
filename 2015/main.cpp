#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <map>

#include "md5.h"

// https://adventofcode.com/2015

namespace AoC
{
    class FileSystem
    {
    public:
        static std::string ReadAllText(const char *filename)
        {
            std::string result;
            std::ifstream inputFile(filename);
            if (inputFile.is_open())
            {
                std::stringstream buffer;
                buffer << inputFile.rdbuf();
                result = buffer.str();
            }
            return result;
        }
        static std::vector<std::string> ReadAllLines(const char *filename)
        {
            std::vector<std::string> result;
            std::ifstream inputFile(filename);
            if (inputFile.is_open())
            {
                std::string line;
                while (std::getline(inputFile, line))
                {
                    result.push_back(std::move(line));
                }
            }
            return result;
        }
    };
}

void day01()
{
    std::string inputData = AoC::FileSystem::ReadAllText("input01.txt");
    int floor = 0;
    int firstBasementEntryPos = 0;
    for (size_t i = 0; i < inputData.length(); i++)
    {
        char c = inputData[i];
        switch (c)
        {
        case '(':
            ++floor;
            break;
        case ')':
            --floor;
            break;

        default:
            break;
        }
        if (floor == -1 && firstBasementEntryPos == 0)
        {
            firstBasementEntryPos = i + 1;
        }
    }
    std::cout << "AoC: Day 01: Final floor:     " << floor << std::endl;
    std::cout << "AoC: Day 01: Enters basement: " << firstBasementEntryPos << std::endl;
    std::cout << std::endl;
}

void day02()
{
    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input02.txt");
    int totalWrappingSqFt = 0;
    int totalRibbonFeet = 0;
    for (size_t i = 0; i < inputData.size(); i++)
    {
        std::string &line = inputData[i];
        std::istringstream lineStream(line);
        int l, w, h;
        char x;
        lineStream >> l >> x >> w >> x >> h;
        totalWrappingSqFt += (2 * l * w) + (2 * w * h) + (2 * h * l);
        totalWrappingSqFt += std::min(std::min(l * w, w * h), h * l);

        std::array<int, 3> sides = {l, w, h};
        std::sort(sides.begin(), sides.end());

        totalRibbonFeet += l * w * h;
        totalRibbonFeet += sides[0];
        totalRibbonFeet += sides[0];
        totalRibbonFeet += sides[1];
        totalRibbonFeet += sides[1];
    }
    std::cout << "AoC: Day 02: Total wrapping square feet: " << totalWrappingSqFt << std::endl;
    std::cout << "AoC: Day 02: Total ribbon feet:   " << totalRibbonFeet << std::endl;
    std::cout << std::endl;
}

void day03()
{
    struct HousePos
    {
    public:
        int x = 0;
        int y = 0;

        void Move(char c)
        {
            switch (c)
            {
            case '^':
                ++y;
                break;
            case '>':
                ++x;
                break;
            case 'v':
                --y;
                break;
            case '<':
                --x;
                break;
            }
        }

        bool operator<(const HousePos &rhs) const
        {
            if (x < rhs.x)
            {
                return true;
            }
            else if (x == rhs.x && y < rhs.y)
            {
                return true;
            }
            return false;
        }
    };
    auto houseVisitor = [&](std::map<HousePos, int> &visits, HousePos pos)
    {
        auto it = visits.find(pos);
        if (it != visits.end())
        {
            ++it->second;
        }
        else
        {
            visits[pos] = 1;
        }
    };
    std::map<HousePos, int> houseVisitsP1;
    std::map<HousePos, int> houseVisitsP2;
    HousePos positions[3] = {0};
    houseVisitor(houseVisitsP1, positions[2]);
    houseVisitor(houseVisitsP2, positions[0]);
    houseVisitor(houseVisitsP2, positions[1]);
    std::string inputData = AoC::FileSystem::ReadAllText("input03.txt");
    for (size_t i = 0; i < inputData.length(); i++)
    {
        // Part 1
        {
            positions[2].Move(inputData[i]);
            houseVisitor(houseVisitsP1, positions[2]);
        }
        // Part 2
        {
            positions[i % 2].Move(inputData[i]);
            houseVisitor(houseVisitsP2, positions[i % 2]);
        }
    }
    std::cout << "AoC: Day 03: Num houses visited P1: " << houseVisitsP1.size() << std::endl;
    std::cout << "AoC: Day 03: Num houses visited P2: " << houseVisitsP2.size() << std::endl;
    std::cout << std::endl;
}

void day04()
{
    const char *fiveZero = "00000";
    const char *sixZero = "000000";
    char data[20];
    int resultP1 = 0;
    int resultP2 = 0;
    for (int i = 0; i < std::numeric_limits<int>::max(); i++)
    {
        sprintf(data, "bgvyzdsv%d", i);
        std::string hash = AoC::MD5::HashBytes(reinterpret_cast<unsigned char *>(data), strlen(data));
        if (resultP1 == 0 && hash.find(fiveZero) == 0)
        {
            resultP1 = i;
        }
        if (resultP2 == 0 && hash.find(sixZero) == 0)
        {
            resultP2 = i;
            break;
        }
    }

    std::cout << "AoC: Day 04: Hash with five zeros: " << resultP1 << std::endl;
    std::cout << "AoC: Day 04: Hash with six zeros:  " << resultP2 << std::endl;
    std::cout << std::endl;
}

int main()
{
    day01();
    day02();
    day03();
    day04();
}

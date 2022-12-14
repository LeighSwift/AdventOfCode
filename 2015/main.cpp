#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <map>
#include <set>

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
        snprintf(data, 20, "bgvyzdsv%d", i);
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

void day05()
{
    auto countVowel = [&](char c)
    {
        return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ? 1 : 0;
    };

    auto isBad = [&](const char *c)
    {
        switch (*c)
        {
        case 'a':
        case 'c':
        case 'p':
        case 'x':
            return *c == (*(c + 1)) - 1;
        }
        return false;
    };

    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input05.txt");
    int niceStringCountP1 = 0;
    int niceStringCountP2 = 0;
    int doubleLetterTable[26 * 26];
    for (size_t i = 0; i < inputData.size(); i++)
    {
        const std::string &line = inputData[i];
        // Part 1
        {
            int vowelCount = 0;
            bool bRepeatLetter = false;
            bool bHasBadSubstring = false;
            for (size_t j = 0; j < line.length(); j++)
            {
                const char c = line[j];
                vowelCount += countVowel(c);
                bRepeatLetter = bRepeatLetter || (j > 0 && line[j] == line[j - 1]);
                bHasBadSubstring = bHasBadSubstring || (j > 0 && isBad(&line[j - 1]));
                if (bHasBadSubstring)
                {
                    break;
                }
            }
            if (!bHasBadSubstring && vowelCount >= 3 && bRepeatLetter)
            {
                ++niceStringCountP1;
            }
        }
        // Part 2
        {
            std::memset(doubleLetterTable, -1, 26 * 26 * 4);
            bool bRepeatLetter = false;
            bool bGoodPair = false;
            std::string pair;
            std::string repeat;
            for (size_t j = 0; j < line.length(); j++)
            {
                bRepeatLetter = bRepeatLetter || (j > 1 && line[j] == line[j - 2]);
                if (j > 0)
                {
                    const char c1 = line[j];
                    const char c0 = line[j - 1];
                    int tableIdx = (c0 - 'a') + (26 * (c1 - 'a'));
                    if (doubleLetterTable[tableIdx] >= 0)
                    {
                        bGoodPair = bGoodPair || (((j - 1) - doubleLetterTable[tableIdx]) >= 2);
                    }
                    else
                    {
                        doubleLetterTable[tableIdx] = j - 1;
                    }
                }
            }
            if (bRepeatLetter && bGoodPair)
            {
                ++niceStringCountP2;
            }
        }
    }

    std::cout << "AoC: Day 05: Num nice string P1: " << niceStringCountP1 << std::endl;
    std::cout << "AoC: Day 05: Num nice string P2: " << niceStringCountP2 << std::endl;
    std::cout << std::endl;
}

void day06()
{
    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input06.txt");
    bool *xmasLights = new bool[1000 * 1000];
    int *xmasLightsP2 = new int[1000 * 1000];
    std::memset(xmasLights, 0, 1000 * 1000 * sizeof(bool));
    std::memset(xmasLightsP2, 0, 1000 * 1000 * sizeof(int));
    const char *turnOn = "turn on";
    const char *turnOff = "turn off";
    const char *toggle = "toggle";
    int numLitLightsP1 = 0;
    int totalBrightnessP2 = 0;
    auto turnOnFn = [&](int x, int y)
    {
        bool &light = xmasLights[x + (y * 1000)];
        numLitLightsP1 = light ? numLitLightsP1 : numLitLightsP1 + 1;
        light = true;
        int &lightP2 = xmasLightsP2[x + (y * 1000)];
        ++lightP2;
        ++totalBrightnessP2;
    };
    auto turnOffFn = [&](int x, int y)
    {
        bool &light = xmasLights[x + (y * 1000)];
        numLitLightsP1 = light ? numLitLightsP1 - 1 : numLitLightsP1;
        light = false;
        int &lightP2 = xmasLightsP2[x + (y * 1000)];
        if (lightP2 > 0)
        {
            --lightP2;
            --totalBrightnessP2;
        }
    };
    auto toggleFn = [&](int x, int y)
    {
        bool &light = xmasLights[x + (y * 1000)];
        numLitLightsP1 = light ? numLitLightsP1 - 1 : numLitLightsP1 + 1;
        light = !light;
        int &lightP2 = xmasLightsP2[x + (y * 1000)];
        ++lightP2;
        ++lightP2;
        ++totalBrightnessP2;
        ++totalBrightnessP2;
    };
    for (size_t i = 0; i < inputData.size(); i++)
    {
        const std::string &line = inputData[i];
        std::istringstream lineStream(line);
        std::function<void(int, int)> fn;
        if (line.rfind(turnOn, 0) == 0)
        {
            lineStream.ignore(strlen(turnOn));
            fn = turnOnFn;
        }
        else if (line.rfind(turnOff, 0) == 0)
        {
            lineStream.ignore(strlen(turnOff));
            fn = turnOffFn;
        }
        else if (line.rfind(toggle, 0) == 0)
        {
            lineStream.ignore(strlen(toggle));
            fn = toggleFn;
        }
        int startX, startY, endX, endY = 0;
        lineStream >> startX;
        lineStream.ignore(1);
        lineStream >> startY;
        lineStream.ignore(9);
        lineStream >> endX;
        lineStream.ignore(1);
        lineStream >> endY;
        for (int x = startX; x <= endX; x++)
        {
            for (int y = startY; y <= endY; y++)
            {
                fn(x, y);
            }
        }
    }
    delete[] xmasLights;
    std::cout << "AoC: Day 06: Num lights on: " << numLitLightsP1 << std::endl;
    std::cout << "AoC: Day 06: Total brightness: " << totalBrightnessP2 << std::endl;
    std::cout << std::endl;
}

void day07()
{
    static std::map<std::string, std::unique_ptr<class GateBase>> wires;

    class GateBase
    {
    public:
        virtual ~GateBase() = default;
        virtual uint16_t CalcResult() = 0;

        uint16_t GetResult()
        {
            if (!m_Result.has_value())
            {
                m_Result = CalcResult();
            }
            return m_Result.value();
        }

        uint16_t GetSignal(const std::string &wireVal)
        {
            auto wire = wires.find(wireVal);
            if (wire != wires.end())
            {
                return wire->second->GetResult();
            }
            return atoi(wireVal.c_str());
        }

        std::optional<uint16_t> m_Result;
    };

    class ConstValue : public GateBase
    {
    public:
        ConstValue(std::string val)
            : m_Val(std::move(val))
        {
        }
        virtual uint16_t CalcResult() override
        {
            return GetSignal(m_Val);
        }

    private:
        const std::string m_Val;
    };

    class AndGate : public GateBase
    {
    public:
        AndGate(std::string lhs, std::string rhs)
            : m_Lhs(std::move(lhs)), m_Rhs(std::move(rhs))
        {
        }
        virtual uint16_t CalcResult() override
        {
            return GetSignal(m_Lhs) & GetSignal(m_Rhs);
        }

    private:
        const std::string m_Lhs;
        const std::string m_Rhs;
    };

    class OrGate : public GateBase
    {
    public:
        OrGate(std::string lhs, std::string rhs)
            : m_Lhs(std::move(lhs)), m_Rhs(std::move(rhs))
        {
        }
        virtual uint16_t CalcResult() override
        {
            return GetSignal(m_Lhs) | GetSignal(m_Rhs);
        }

    private:
        const std::string m_Lhs;
        const std::string m_Rhs;
    };

    class NotGate : public GateBase
    {
    public:
        NotGate(std::string input)
            : m_Input(std::move(input))
        {
        }
        virtual uint16_t CalcResult() override
        {
            return ~GetSignal(m_Input);
        }

    private:
        const std::string m_Input;
    };

    class LShiftGate : public GateBase
    {
    public:
        LShiftGate(std::string lhs, std::string rhs)
            : m_Lhs(std::move(lhs)), m_Rhs(std::string(rhs))
        {
        }
        virtual uint16_t CalcResult() override
        {
            return GetSignal(m_Lhs) << GetSignal(m_Rhs);
        }

    private:
        const std::string m_Lhs;
        const std::string m_Rhs;
    };

    class RShiftGate : public GateBase
    {
    public:
        RShiftGate(std::string lhs, std::string rhs)
            : m_Lhs(std::move(lhs)), m_Rhs(std::string(rhs))
        {
        }
        virtual uint16_t CalcResult() override
        {
            return GetSignal(m_Lhs) >> GetSignal(m_Rhs);
        }

    private:
        const std::string m_Lhs;
        const std::string m_Rhs;
    };

    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input07.txt");
    for (size_t i = 0; i < inputData.size(); i++)
    {
        const std::string &line = inputData[i];
        std::istringstream lineStream(line);
        if (line.find("AND") != std::string::npos)
        {
            // x AND y -> d
            std::string wire, lhs, rhs;
            lineStream >> lhs;
            lineStream.ignore(5);
            lineStream >> rhs;
            lineStream.ignore(4);
            lineStream >> wire;
            wires[wire].reset(new AndGate(lhs, rhs));
        }
        else if (line.find("OR") != std::string::npos)
        {
            // x OR y -> d
            std::string wire, lhs, rhs;
            lineStream >> lhs;
            lineStream.ignore(4);
            lineStream >> rhs;
            lineStream.ignore(4);
            lineStream >> wire;
            wires[wire].reset(new OrGate(lhs, rhs));
        }
        else if (line.find("LSHIFT") != std::string::npos)
        {
            // x LSHIFT y -> d
            std::string wire, lhs, rhs;
            lineStream >> lhs;
            lineStream.ignore(8);
            lineStream >> rhs;
            lineStream.ignore(4);
            lineStream >> wire;
            wires[wire].reset(new LShiftGate(lhs, rhs));
        }
        else if (line.find("RSHIFT") != std::string::npos)
        {
            // x RSHIFT y -> d
            std::string wire, lhs, rhs;
            lineStream >> lhs;
            lineStream.ignore(8);
            lineStream >> rhs;
            lineStream.ignore(4);
            lineStream >> wire;
            wires[wire].reset(new RShiftGate(lhs, rhs));
        }
        else if (line.find("NOT") != std::string::npos)
        {
            // NOT x -> h
            std::string val, wire;
            lineStream.ignore(4);
            lineStream >> val;
            lineStream.ignore(4);
            lineStream >> wire;
            wires[wire].reset(new NotGate(val));
        }
        else
        {
            // x -> y
            std::string val, wire;
            lineStream >> val;
            lineStream.ignore(4);
            lineStream >> wire;
            wires[wire].reset(new ConstValue(val));
        }
    }

    // Part 1: Get wire A
    uint16_t wireA = wires["a"]->GetResult();
    std::cout << "AoC: Day 07: Wire a P1: " << wireA << std::endl;
    // Part 2: Make wire b and constant of wireA value, reset all, get wire A again.
    char wireAStr[100];
    snprintf(wireAStr, 100, "%u", wireA);
    wires["b"].reset(new ConstValue(wireAStr));
    for (auto &&wire : wires)
    {
        wire.second->m_Result.reset();
    }
    std::cout << "AoC: Day 07: Wire a P2: " << wires["a"]->GetResult() << std::endl;
    std::cout << std::endl;
}

void day08()
{
    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input08.txt");
    auto countChars = [](const std::string &str)
    {
        int numChars = 0;
        bool bEscaped = false;
        for (size_t i = 0; i < str.length(); i++)
        {
            if (bEscaped)
            {
                if (str[i] == 'x')
                {
                    i += 2;
                    ++numChars;
                }
                else
                {
                    ++numChars;
                }
                bEscaped = false;
            }
            else
            {
                if (str[i] == '\\')
                {
                    bEscaped = true;
                }
                else
                {
                    ++numChars;
                }
            }
        }
        return numChars;
    };
    auto encodeString = [](const std::string &str)
    {
        std::string newStr = "\"";
        for (auto &&c : str)
        {
            if (c == '\\')
            {
                newStr += "\\\\";
            }
            else if (c == '"')
            {
                newStr += "\\\"";
            }
            else
            {
                newStr += c;
            }
        }
        newStr += "\"";
        return newStr;
    };
    int totalCodeSize = 0;
    int totalCodeSizeP2 = 0;
    int totalCharacters = 0;
    for (size_t i = 0; i < inputData.size(); i++)
    {
        const std::string &line = inputData[i];
        totalCodeSize += line.length();
        totalCharacters += countChars(line.substr(1, line.length() - 2));
        totalCodeSizeP2 += encodeString(line).length();
    }

    std::cout << "AoC: Day 08: Code - Chars   = " << totalCodeSize - totalCharacters << std::endl;
    std::cout << "AoC: Day 08: NewCode - Code = " << totalCodeSizeP2 - totalCodeSize << std::endl;
    std::cout << std::endl;
}

void day09()
{
    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input09.txt");
    std::set<std::string> locations;
    std::map<std::string, int> distances;
    for (size_t i = 0; i < inputData.size(); i++)
    {
        std::string &line = inputData[i];
        std::istringstream lineStream(line);
        std::string locA, locB;
        int dist;
        lineStream >> locA;
        lineStream.ignore(4);
        lineStream >> locB;
        lineStream.ignore(3);
        lineStream >> dist;
        distances[locA + locB] = dist;
        distances[locB + locA] = dist;
        locations.insert(locA);
        locations.insert(locB);
    }
    // Brute force part 1
    class LocNode
    {
    public:
        std::string m_loc;
        std::unique_ptr<LocNode[]> m_Next;
        int m_NextNum = 0;
    };
    // Using a local hangs lldb on macOS :( so allocations are necessary
    std::unique_ptr<LocNode> root(new LocNode());
    std::function<void(LocNode &, const std::set<std::string> &)> populate;
    populate = [&](LocNode &node, const std::set<std::string> &children)
    {
        node.m_NextNum = children.size();
        node.m_Next.reset(new LocNode[node.m_NextNum]);
        for (int i = 0; i < node.m_NextNum; i++)
        {
            node.m_Next[i].m_loc =   ..push_back(LocNode(child));
            std::set<std::string> nextChildren = children;
            nextChildren.erase(child);
            if (nextChildren.size() > 0)
            {
                populate(node.m_Next.back(), nextChildren);
            }
        }
    };
    populate(*root, locations);

    int shortestRoute = std::numeric_limits<int>::max();
    int longestRoute = 0;
    std::function<void(const LocNode &, const LocNode &, int, std::string)> calculate;
    calculate = [&](const LocNode &node, const LocNode &prev, int accum, std::string Route)
    {
        accum += distances[prev.m_loc + node.m_loc];
        Route += " -> ";
        Route += node.m_loc;
        if (node.m_Next.size() == 0)
        {
            Route += " = ";
            char numStr[5];
            snprintf(numStr, 5, "%d", accum);
            Route += numStr;
            if (accum < shortestRoute)
            {
                shortestRoute = accum;
            }
            if (accum > longestRoute)
            {
                longestRoute = accum;
            }
            std::cout <<Route << std::endl;
        }
        else
        {
            for (const LocNode &next : node.m_Next)
            {
                calculate(next, node, accum, Route);
            }
        }
    };

    for (const LocNode &start : root->m_Next)
    {
        for (const LocNode &next : start.m_Next)
        {
            calculate(next, start, 0, start.m_loc);
        }
    }
    std::cout << std::endl;

    std::cout << "AoC: Day 09 Part1: " << shortestRoute << std::endl;
    std::cout << "AoC: Day 09 Part2: " << longestRoute << std::endl;
    std::cout << std::endl;
}

void day10()
{
    // std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input10.txt");
    // for (size_t i = 0; i < inputData.size(); i++)
    // {
    //     std::string &line = inputData[i];
    // }
}

void day11()
{
    // std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input11.txt");
    // for (size_t i = 0; i < inputData.size(); i++)
    // {
    //     std::string &line = inputData[i];
    // }
}

void day12()
{
    // std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input12.txt");
    // for (size_t i = 0; i < inputData.size(); i++)
    // {
    //     std::string &line = inputData[i];
    // }
}

void day13()
{
    // std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input13.txt");
    // for (size_t i = 0; i < inputData.size(); i++)
    // {
    //     std::string &line = inputData[i];
    // }
}

void day14()
{
    // std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input14.txt");
    // for (size_t i = 0; i < inputData.size(); i++)
    // {
    //     std::string &line = inputData[i];
    // }
}

void day15()
{
    // std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input15.txt");
    // for (size_t i = 0; i < inputData.size(); i++)
    // {
    //     std::string &line = inputData[i];
    // }
}

void day16()
{
    // std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input16.txt");
    // for (size_t i = 0; i < inputData.size(); i++)
    // {
    //     std::string &line = inputData[i];
    // }
}

void day17()
{
    // std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input17.txt");
    // for (size_t i = 0; i < inputData.size(); i++)
    // {
    //     std::string &line = inputData[i];
    // }
}

void day18()
{
    // std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input18.txt");
    // for (size_t i = 0; i < inputData.size(); i++)
    // {
    //     std::string &line = inputData[i];
    // }
}

void day19()
{
    // std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input19.txt");
    // for (size_t i = 0; i < inputData.size(); i++)
    // {
    //     std::string &line = inputData[i];
    // }
}

void day20()
{
    // std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input20.txt");
    // for (size_t i = 0; i < inputData.size(); i++)
    // {
    //     std::string &line = inputData[i];
    // }
}

void day21()
{
    // std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input21.txt");
    // for (size_t i = 0; i < inputData.size(); i++)
    // {
    //     std::string &line = inputData[i];
    // }
}

void day22()
{
    // std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input22.txt");
    // for (size_t i = 0; i < inputData.size(); i++)
    // {
    //     std::string &line = inputData[i];
    // }
}

void day23()
{
    // std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input23.txt");
    // for (size_t i = 0; i < inputData.size(); i++)
    // {
    //     std::string &line = inputData[i];
    // }
}

void day24()
{
    // std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input24.txt");
    // for (size_t i = 0; i < inputData.size(); i++)
    // {
    //     std::string &line = inputData[i];
    // }
}

void day25()
{
    // std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input25.txt");
    // for (size_t i = 0; i < inputData.size(); i++)
    // {
    //     std::string &line = inputData[i];
    // }
}

int main()
{
    day01();
    day02();
    day03();
    day04();
    day05();
    day06();
    day07();
    day08();
    day09();
    day10();
    day11();
    day12();
    day13();
    day14();
    day15();
    day16();
    day17();
    day18();
    day19();
    day20();
    day21();
    day22();
    day23();
    day24();
    day25();
}

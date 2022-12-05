#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <numeric>
#include <algorithm>
#include <map>

// https://adventofcode.com/2022

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
    int elfCalories = 0;
    std::vector<int> topThree{0, 0, 0};

    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input01.txt");
    for (size_t i = 0; i < inputData.size(); i++)
    {
        std::string &line = inputData[i];
        if (line.length() == 0)
        {
            topThree[0] = std::max(topThree[0], elfCalories);
            std::sort(topThree.begin(), topThree.end());
            elfCalories = 0;
            continue;
        }
        else
        {
            elfCalories += std::atoi(line.c_str());
        }
    }
    topThree[0] = std::max(topThree[0], elfCalories);
    std::sort(topThree.begin(), topThree.end());

    std::cout << "AoC: Day 01: Highest:          " << topThree[2] << std::endl;
    std::cout << "AoC: Day 01: Sum of top three: " << std::accumulate(topThree.begin(), topThree.end(), 0) << std::endl;
    std::cout << std::endl;
}

void day02()
{
    std::map<char, int> shapeScores;
    shapeScores['A'] = 1;
    shapeScores['B'] = 2;
    shapeScores['C'] = 3;
    std::map<char, char> winningMoves;
    winningMoves['A'] = 'B';
    winningMoves['B'] = 'C';
    winningMoves['C'] = 'A';
    std::map<char, char> losingMoves;
    losingMoves['A'] = 'C';
    losingMoves['B'] = 'A';
    losingMoves['C'] = 'B';

    int totalScoreP1 = 0;
    int totalScoreP2 = 0;
    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input02.txt");
    for (size_t i = 0; i < inputData.size(); i++)
    {
        std::string &line = inputData[i];
        char opponentMove = line[0];
        // Part 1
        {
            char yourMove = line[2] - 23;

            // First do total score for story part 1 incorrect assumption that 'X' == 'A' etc
            totalScoreP1 += shapeScores[yourMove];
            if (opponentMove == yourMove)
            {
                totalScoreP1 += 3;
            }
            else if (yourMove == winningMoves[opponentMove])
            {
                totalScoreP1 += 6;
            }
        }
        // Part 2 - Now figure out correct answer part 2.
        {
            char yourMove;
            switch (line[2])
            {
            case 'X':
                yourMove = losingMoves[opponentMove];
                break;
            case 'Y':
                yourMove = opponentMove;
                totalScoreP2 += 3;
                break;
            case 'Z':
                yourMove = winningMoves[opponentMove];
                totalScoreP2 += 6;
                break;
            }
            totalScoreP2 += shapeScores[yourMove];
        }
    }

    std::cout << "AoC: Day 02: Total score part 1: " << totalScoreP1 << std::endl;
    std::cout << "AoC: Day 02: Total score part 2: " << totalScoreP2 << std::endl;
    std::cout << std::endl;
}

void day03()
{
    auto getPriority = [&](char c)
    {
        if (c <= 'Z')
        {
            return (c - 'A') + 27;
        }
        else
        {
            return (c - 'a') + 1;
        }
    };
    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input03.txt");
    int totalPriorityP1 = 0;
    int totalPriorityP2 = 0;
    for (size_t i = 0; i < inputData.size(); i++)
    {
        // Part 1
        {
            std::string &line = inputData[i];
            const size_t compartmentSize = line.length() / 2;
            std::array<char, 52> table{0};
            std::for_each(line.begin(), line.end() - compartmentSize, [&](char c)
                          { table[getPriority(c) - 1] = 1; });
            totalPriorityP1 += getPriority(*std::find_if(line.begin() + compartmentSize, line.end(), [&](char c)
                                                         { return table[getPriority(c) - 1] == 1; }));
        }
        // Part 2
        {
            if (i % 3 == 2)
            {
                std::array<char, 52> table{0};
                std::string &line1 = inputData[i - 2];
                std::string &line2 = inputData[i - 1];
                std::string &line3 = inputData[i];
                std::for_each(line1.begin(), line1.end(), [&](char c)
                              { table[getPriority(c) - 1] |= 1; });
                std::for_each(line2.begin(), line2.end(), [&](char c)
                              { table[getPriority(c) - 1] |= 2; });
                totalPriorityP2 += getPriority(*std::find_if(line3.begin(), line3.end(), [&](char c)
                                                             { return table[getPriority(c) - 1] == 3; }));
            }
        }
    }

    std::cout << "AoC: Day 03: Total priority P1: " << totalPriorityP1 << std::endl;
    std::cout << "AoC: Day 03: Total priority P2: " << totalPriorityP2 << std::endl;
    std::cout << std::endl;
}

void day04()
{
    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input04.txt");
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

void day05()
{
    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input05.txt");
    // Every line is the correct number of characters to print the crates even if they end with whitespace.
    std::vector<std::string> stacksP1;
    std::vector<std::string> stacksP2;
    bool bParsedStacks = false;
    for (size_t i = 0; i < inputData.size(); i++)
    {
        std::string &line = inputData[i];
        if (!bParsedStacks)
        {
            const int numStacks = (inputData[i].length() + 1) / 4;
            while (stacksP1.size() < numStacks)
            {
                stacksP1.push_back("");
            }
            for (size_t stackIdx = 0; stackIdx < numStacks; stackIdx++)
            {
                char crate = line[(stackIdx * 4) + 1];
                // the numbers are the end f the list
                if (crate == '1')
                {
                    ++i;
                    bParsedStacks = true;
                    stacksP2 = stacksP1;
                    break;
                }
                // no crate
                else if (crate != ' ')
                {
                    stacksP1[stackIdx].insert(stacksP1[stackIdx].begin(), crate);
                }
            }
        }
        else
        {
            std::istringstream lineStream(line);
            int num, from, to;
            lineStream.ignore(5);
            lineStream >> num;
            lineStream.ignore(6);
            lineStream >> from;
            lineStream.ignore(4);
            lineStream >> to;
            --from;
            --to;
            // Part 1
            for (size_t j = 0; j < num; j++)
            {
                stacksP1[to].push_back(stacksP1[from].back());
                stacksP1[from].pop_back();
            }
            // Part 2
            stacksP2[to] += &stacksP2[from][stacksP2[from].length() - num];
            stacksP2[from] = stacksP2[from].substr(0, stacksP2[from].length() - num);
        }
    }

    std::string topsP1;
    std::string topsP2;
    for (size_t i = 0; i < stacksP1.size(); i++)
    {
        topsP1 += stacksP1[i].back();
        topsP2 += stacksP2[i].back();
    }

    std::cout << "AoC: Day 05: : " << topsP1 << std::endl;
    std::cout << "AoC: Day 05: : " << topsP2 << std::endl;
    std::cout << std::endl;
}

int main()
{
    day01();
    day02();
    day03();
    day04();
    day05();
}

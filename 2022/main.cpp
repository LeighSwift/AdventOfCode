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
        if (line.length() == 0 || (i + 1) == inputData.size())
        {
            topThree[0] = std::max(topThree[0], elfCalories);
            std::sort(topThree.begin(), topThree.end());
            elfCalories = 0;
            continue;
        }
        elfCalories += std::atoi(line.c_str());
    }

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

int main()
{
    day01();
    day02();
}

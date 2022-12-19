#include "Common.h"

void Day02()
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
    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("Data/input02.txt");
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

AoC::DaySetter DaySetter02(2, Day02);
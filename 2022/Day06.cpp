#include "Common.h"

void Day06()
{
    auto addChar = [&](char c, char *table)
    {
        ++table[c - 'a'];
    };
    auto removeChar = [&](char c, char *table)
    {
        --table[c - 'a'];
    };
    auto checkAgainstTable = [&](char *chars, int num, char *table)
    {
        for (size_t j = 0; j < num; j++)
        {
            int count = table[chars[j] - 'a'];
            assert(count > 0);
            if (count != 1)
            {
                return false;
            }
        }
        return true;
    };
    char *charTableP1 = new char[26];
    char *charTableP2 = new char[26];
    std::memset(charTableP1, 0, 26);
    std::memset(charTableP2, 0, 26);
    int answerP1 = 0;
    int answerP2 = 0;
    std::string inputData = AoC::FileSystem::ReadAllText("Data/input06.txt");
    for (int i = 0; i < inputData.length(); i++)
    {
        // Track tables
        addChar(inputData[i], charTableP1);
        addChar(inputData[i], charTableP2);
        if (i >= 4)
        {
            removeChar(inputData[i - 4], charTableP1);
        }
        if (i >= 14)
        {
            removeChar(inputData[i - 14], charTableP2);
        }

        // Check part 1
        if (i >= 3 && answerP1 == 0)
        {
            if (checkAgainstTable(&inputData[i - 3], 4, charTableP1))
            {
                answerP1 = i + 1;
            }
        }
        // Check part 2 (no point until after we find part 1)
        else if (i >= 13 && answerP2 == 0)
        {
            if (checkAgainstTable(&inputData[i - 13], 14, charTableP2))
            {
                answerP2 = i + 1;
                break;
            }
        }
    }
    delete[] charTableP1;
    delete[] charTableP2;

    std::cout << "AoC: Day 06: First signal:  " << answerP1 << std::endl;
    std::cout << "AoC: Day 06: First message: " << answerP2 << std::endl;
    std::cout << std::endl;
}

AoC::DaySetter DaySetter06(6, Day06);
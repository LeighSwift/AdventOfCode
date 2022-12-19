#include "Common.h"

void Day01()
{
    int elfCalories = 0;
    std::vector<int> topThree{0, 0, 0};

    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("Data/input01.txt");
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

AoC::DaySetter DaySetter01(1, Day01);
#include "Common.h"

void Day05()
{
    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("Data/input05.txt");
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
                // The numbers are the end of the list
                if (crate == '1')
                {
                    ++i;
                    bParsedStacks = true;
                    stacksP2 = stacksP1;
                    break;
                }
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

AoC::DaySetter DaySetter05(5, Day05);
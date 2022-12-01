#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

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
        if(floor == -1 && firstBasementEntryPos == 0)
        {
            firstBasementEntryPos = i+1;
        }
    }
    std::cout << "AoC: Day 01: Final floor:     " << floor << std::endl;
    std::cout << "AoC: Day 01: Enters basement: " << firstBasementEntryPos << std::endl;
}

void day02()
{
    
}

int main()
{
    day01();
}

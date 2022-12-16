#include "Common.h"

void Day13()
{
    class Day13
    {
    public:
        static void FindNextElem(const char *&str)
        {
            if (*str == '[' || *str == ',')
            {
                ++str;
            }
        }
        static void SkipInteger(const char *&str)
        {
            while (*str >= '0' && *str <= '9')
            {
                ++str;
            }
        }
        static int Eval(int lhs, int rhs)
        {
            if (lhs < rhs)
            {
                return 0;
            }
            else if (lhs == rhs)
            {
                return 1;
            }
            else
            {
                return 2;
            }
        }
        static int Eval(const char *&lhs, int rhs)
        {
            char newArr[10];
            snprintf(newArr, 10, "[%d]", rhs);
            const char *newArrPtr = newArr;
            return Eval(lhs, newArrPtr);
        }
        static int Eval(int lhs, const char *&rhs)
        {
            char newArr[10];
            snprintf(newArr, 10, "[%d]", lhs);
            const char *newArrPtr = newArr;
            return Eval(newArrPtr, rhs);
        }
        static int Eval(const char *&lhs, const char *&rhs)
        {
            if (*lhs == ']' && *rhs != ']')
            {
                return 0;
            }
            else if (*lhs != ']' && *rhs == ']')
            {
                return 2;
            }
            else if (*lhs == '[' && *rhs == '[')
            {
                int rtn = 1;
                while (rtn == 1)
                {
                    FindNextElem(lhs);
                    FindNextElem(rhs);
                    if (*lhs == ']' && *rhs == ']')
                    {
                        ++lhs;
                        ++rhs;
                        return 1;
                    }
                    rtn = Eval(lhs, rhs);
                }
                return rtn;
            }
            else if (*lhs == '[' && *rhs != '[')
            {
                int rhsInt = atoi(rhs);
                SkipInteger(rhs);
                return Eval(lhs, rhsInt);
            }
            else if (*lhs != '[' && *rhs == '[')
            {
                int lhsInt = atoi(lhs);
                SkipInteger(lhs);
                return Eval(lhsInt, rhs);
            }
            else
            {
                int lhsInt = atoi(lhs);
                int rhsInt = atoi(rhs);
                SkipInteger(lhs);
                SkipInteger(rhs);
                return Eval(lhsInt, rhsInt);
            }
        }
        static int Eval(const std::string &inLhs, const std::string &inRhs)
        {
            const char *lhs = inLhs.c_str();
            const char *rhs = inRhs.c_str();
            return Eval(lhs, rhs);
        }
    };

    class Packet
    {
    public:
        bool operator<(const Packet &rhs) const
        {
            return Day13::Eval(message, rhs.message) == 0;
        }

        bool operator==(const Packet &other) const
        {
            return message == other.message;
        }

        std::string message;
    };

    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("Data/input13.txt");
    std::vector<int> correctOrder;
    std::vector<int> incorrectOrder;
    std::vector<Packet> packets;
    int index = 1;
    for (size_t i = 1; i < inputData.size(); i += 3)
    {
        std::string &left = inputData[i - 1];
        std::string &right = inputData[i];

        packets.push_back({left});
        packets.push_back({right});

        int result = Day13::Eval(left, right);
        if (result == 0)
        {
            correctOrder.push_back(index);
        }
        else if (result == 2)
        {
            incorrectOrder.push_back(index);
        }
        ++index;
        assert(result != 1);
    }

    int correctIndexSum = std::accumulate(correctOrder.begin(), correctOrder.end(), 0);

    Packet divOne = {"[[2]]"};
    Packet divTwo = {"[[6]]"};
    packets.push_back(divOne);
    packets.push_back(divTwo);
    std::sort(packets.begin(), packets.end());
    int divOneIndex = std::find(packets.begin(), packets.end(), divOne) - packets.begin() + 1;
    int divTwoIndex = std::find(packets.begin(), packets.end(), divTwo) - packets.begin() + 1;

    std::cout << "AoC: Day 13: Part 1:  " << correctIndexSum << std::endl;
    std::cout << "AoC: Day 13: Part 2:  " << divOneIndex * divTwoIndex << std::endl;
    std::cout << std::endl;
}

AoC::DaySetter DaySetter13(13, Day13);
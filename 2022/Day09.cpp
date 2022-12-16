#include "Common.h"

void Day09()
{
    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("Data/input09.txt");
    class Knot
    {
    public:
        int x = 0;
        int y = 0;

        bool operator<(const Knot &rhs) const
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

        bool IsAdjacent(const Knot &other) const
        {
            return abs(x - other.x) <= 1 && abs(y - other.y) <= 1;
        }

        void Step(char dir)
        {
            switch (dir)
            {
            case 'R':
                ++x;
                break;
            case 'L':
                --x;
                break;
            case 'U':
                ++y;
                break;
            case 'D':
                --y;
                break;
            }
        }

        void Follow(const Knot &other)
        {
            if (!IsAdjacent(other))
            {
                if (x > other.x)
                {
                    x--;
                }
                else if (x < other.x)
                {
                    x++;
                }
                if (y > other.y)
                {
                    y--;
                }
                else if (y < other.y)
                {
                    y++;
                }
            }
        }
    };
    class Rope
    {
    public:
        Rope(int numKnots)
        {
            m_Knots.insert(m_Knots.begin(), numKnots, Knot());
            tailHistory[m_Knots.back()] = 1;
        }

        void MoveHead(char dir)
        {
            m_Knots.front().Step(dir);
            for (size_t i = 1; i < m_Knots.size(); i++)
            {
                m_Knots[i].Follow(m_Knots[i - 1]);
            }

            auto prevLoc = tailHistory.find(m_Knots.back());
            if (prevLoc != tailHistory.end())
            {
                prevLoc->second++;
            }
            else
            {
                tailHistory[m_Knots.back()] = 1;
            }
        }
        std::vector<Knot> m_Knots;
        std::map<Knot, int> tailHistory;
    };
    Rope part1(2);
    Rope part2(10);
    for (size_t i = 0; i < inputData.size(); i++)
    {
        std::string &line = inputData[i];
        std::istringstream lineStream(inputData[i]);
        char dir;
        int num;
        lineStream >> dir >> num;
        for (int i = 0; i < num; i++)
        {
            part1.MoveHead(dir);
            part2.MoveHead(dir);
        }
    }

    std::cout << "AoC: Day 09: Part 1 locations tail visited:  " << part1.tailHistory.size() << std::endl;
    std::cout << "AoC: Day 09: Part 2 locations tail visited:  " << part2.tailHistory.size() << std::endl;
    std::cout << std::endl;
}

AoC::DaySetter DaySetter09(9, Day09);
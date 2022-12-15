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

void day06()
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
    std::string inputData = AoC::FileSystem::ReadAllText("input06.txt");
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

void day07()
{
    enum class EObjectType
    {
        Directory = 0,
        File
    };

    class ObjectNode
    {
    public:
        ObjectNode(ObjectNode *parent)
            : m_Parent(parent)
        {
        }
        virtual ~ObjectNode() = default;
        virtual EObjectType GetType() = 0;
        virtual uint64_t GetSize() = 0;

        std::map<std::string, std::unique_ptr<ObjectNode>> m_Contents;
        ObjectNode *m_Parent;
    };

    class Directory : public ObjectNode
    {
    public:
        Directory(ObjectNode *parent)
            : ObjectNode(parent)
        {
        }
        virtual EObjectType GetType() { return EObjectType::Directory; }
        virtual uint64_t GetSize()
        {
            if (!m_Size.has_value())
            {
                // Recurse all
                uint64_t size = 0;
                for (auto &&i : m_Contents)
                {
                    size += i.second->GetSize();
                }
                m_Size = size;
            }
            return m_Size.value();
        }

    private:
        std::optional<uint64_t> m_Size;
    };

    class File : public ObjectNode
    {
    public:
        File(ObjectNode *parent, uint64_t size)
            : ObjectNode(parent), m_Size(size)
        {
        }
        virtual EObjectType GetType() { return EObjectType::File; }
        virtual uint64_t GetSize() { return m_Size; }

    private:
        uint64_t m_Size = 0;
    };

    std::unique_ptr<ObjectNode> Root(new Directory(nullptr));
    ObjectNode *currNode = Root.get();
    currNode->m_Contents["/"] = std::unique_ptr<ObjectNode>(new Directory(currNode));

    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input07.txt");
    for (size_t i = 0; i < inputData.size(); i++)
    {
        std::string &line = inputData[i];
        if (line.rfind("$ cd ", 0) == 0)
        {
            std::string dirName = line.substr(5);
            if (dirName == "..")
            {
                currNode = currNode->m_Parent;
            }
            else
            {
                auto child = currNode->m_Contents.find(dirName);
                assert(child != currNode->m_Contents.end());
                assert(child->second->GetType() == EObjectType::Directory);
                currNode = child->second.get();
            }
        }
        else if (line == "$ ls")
        {
            i++;
            for (; i < inputData.size(); i++)
            {
                std::string &lsResult = inputData[i];
                if (lsResult.rfind("$", 0) == 0)
                {
                    i--;
                    break;
                }
                else if (lsResult.rfind("dir ", 0) == 0)
                {
                    std::string dirName = lsResult.substr(4);
                    if (currNode->m_Contents.find(dirName) == currNode->m_Contents.end())
                    {
                        Directory *newDir = new Directory(currNode);
                        currNode->m_Contents[dirName] = std::unique_ptr<ObjectNode>(newDir);
                    }
                }
                else if (lsResult.length() > 0)
                {
                    std::istringstream lineStream(lsResult);
                    uint64_t fileSize;
                    std::string fileName;
                    lineStream >> fileSize;
                    lineStream.ignore();
                    lineStream >> fileName;
                    File *newFile = new File(currNode, fileSize);
                    currNode->m_Contents[fileName] = std::unique_ptr<ObjectNode>(newFile);
                }
                else if (lsResult.length() > 0)
                {
                    assert(false);
                }
            }
        }
        else if (line.length() > 0)
        {
            assert(false);
        }
    }

    // Part 1, total directories with sizes <= 100000 (yes, we'll count files twice)
    std::function<uint64_t(ObjectNode *)> countPart1;
    countPart1 = [&](ObjectNode *node)
    {
        uint64_t result = 0;
        if (node->GetType() == EObjectType::Directory && node->GetSize() <= 100000)
        {
            result += node->GetSize();
        }
        for (auto &&obj : node->m_Contents)
        {
            if (obj.second->GetType() == EObjectType::Directory)
            {
                result += countPart1(obj.second.get());
            }
        }
        return result;
    };

    // Part 2, drive == 70000000 B, need 30000000 B, find size smallest dir to delete
    uint64_t spaceFree = 70000000 - Root->GetSize();
    uint64_t spaceNeeded = 30000000 - spaceFree;
    uint64_t smallest = std::numeric_limits<uint64_t>::max();
    std::function<void(ObjectNode *)> findPart2;
    findPart2 = [&](ObjectNode *node)
    {
        if (node->GetType() == EObjectType::Directory && node->GetSize() >= spaceNeeded && node->GetSize() < smallest)
        {
            smallest = node->GetSize();
        }
        for (auto &&obj : node->m_Contents)
        {
            if (obj.second->GetType() == EObjectType::Directory)
            {
                findPart2(obj.second.get());
            }
        }
    };
    findPart2(Root->m_Contents["/"].get());
    std::cout << "AoC: Day 07: Part 1 total:  " << countPart1(Root->m_Contents["/"].get()) << std::endl;
    std::cout << "AoC: Day 07: Part 2 size:   " << smallest << std::endl;
    std::cout << std::endl;
}

void day08()
{
    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input08.txt");
    const int numRows = inputData.size();
    const int numCols = inputData[0].length();
    char *treeData = new char[numRows * numCols];
    char *treeVisible = new char[numRows * numCols];
    // 8-bits: 0000NESW
    const char northVisible = 8;
    const char eastVisible = 4;
    const char southVisible = 2;
    const char westVisible = 1;
    std::memset(treeVisible, 0, numRows * numCols * sizeof(char));
    auto getTreeHeight = [&](int x, int y) -> char *
    {
        return treeData + (y * numRows) + x;
    };
    auto getTreeVisible = [&](int x, int y) -> char *
    {
        return treeVisible + (y * numRows) + x;
    };
    for (size_t y = 0; y < inputData.size(); y++)
    {
        for (size_t x = 0; x < inputData[y].length(); x++)
        {
            *getTreeHeight(x, y) = inputData[y][x] - '0';
        }
    }
    // Set edges visible
    for (int y = 0; y < numRows; y++)
    {
        *getTreeVisible(0, y) = 0xf;
        *getTreeVisible(numCols - 1, y) = 0xf;
    }
    for (int x = 0; x < numCols; x++)
    {
        *getTreeVisible(x, 0) = 0xf;
        *getTreeVisible(x, numRows - 1) = 0xf;
    }
    // Process north
    for (int x = 1; x < numRows - 1; x++)
    {
        int highest = *getTreeHeight(x, numRows - 1);
        for (int y = numRows - 2; y > 0; y--)
        {
            int treeHeight = *getTreeHeight(x, y);
            if (treeHeight > highest)
            {
                *getTreeVisible(x, y) |= northVisible;
                highest = treeHeight;
            }
            // Can stop if we hit a 9
            if (treeHeight == 9)
            {
                break;
            }
        }
    }
    // Process east
    for (int y = 1; y < numRows - 1; y++)
    {
        int highest = *getTreeHeight(0, y);
        for (int x = 1; x < numCols - 1; x++)
        {
            int treeHeight = *getTreeHeight(x, y);
            if (treeHeight > highest)
            {
                *getTreeVisible(x, y) |= eastVisible;
                highest = treeHeight;
            }
            // Can stop if we hit a 9
            if (treeHeight == 9)
            {
                break;
            }
        }
    }
    // Process south
    for (int x = 1; x < numRows - 1; x++)
    {
        int highest = *getTreeHeight(x, 0);
        for (int y = 1; y < numCols - 1; y++)
        {
            int treeHeight = *getTreeHeight(x, y);
            if (treeHeight > highest)
            {
                *getTreeVisible(x, y) |= southVisible;
                highest = treeHeight;
            }
            // Can stop if we hit a 9
            if (treeHeight == 9)
            {
                break;
            }
        }
    }
    // Process west
    for (int y = 1; y < numRows - 1; y++)
    {
        int highest = *getTreeHeight(numCols - 1, y);
        for (int x = numCols - 2; x > 0; x--)
        {
            int treeHeight = *getTreeHeight(x, y);
            if (treeHeight > highest)
            {
                *getTreeVisible(x, y) |= westVisible;
                highest = treeHeight;
            }
            // Can stop if we hit a 9
            if (treeHeight == 9)
            {
                break;
            }
        }
    }

    // Part 1 - how many visible trees
    int numVisible = 0;
    for (int i = 0; i < numRows * numCols; i++)
    {
        if (treeVisible[i] > 0)
        {
            ++numVisible;
        }
    }

    // Part 2 - Tree score for visibility n * e * s * w
    int highestScore = 0;
    for (int x = 0; x < numCols; x++)
    {
        for (int y = 0; y < numRows; y++)
        {
            int treeHeight = *getTreeHeight(x, y);
            // North
            int countN = 0;
            for (int yy = y - 1; yy >= 0; yy--)
            {
                ++countN;
                int nextTree = *getTreeHeight(x, yy);
                if (nextTree >= treeHeight)
                {
                    break;
                }
            }
            // East
            int countE = 0;
            for (int xx = x + 1; xx < numCols; xx++)
            {
                ++countE;
                int nextTree = *getTreeHeight(xx, y);
                if (nextTree >= treeHeight)
                {
                    break;
                }
            }
            // South
            int countS = 0;
            for (int yy = y + 1; yy < numRows; yy++)
            {
                ++countS;
                int nextTree = *getTreeHeight(x, yy);
                if (nextTree >= treeHeight)
                {
                    break;
                }
            }
            // West
            int countW = 0;
            for (int xx = x - 1; xx >= 0; xx--)
            {
                ++countW;
                int nextTree = *getTreeHeight(xx, y);
                if (nextTree >= treeHeight)
                {
                    break;
                }
            }
            // Total
            int score = countN * countE * countS * countW;
            if (score > highestScore)
            {
                highestScore = score;
            }
        }
    }

    std::cout << "AoC: Day 08: Part 1 total visible:  " << numVisible << std::endl;
    std::cout << "AoC: Day 08: Part 2 highest score:  " << highestScore << std::endl;
    std::cout << std::endl;
    delete[] treeData;
}

void day09()
{
    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input09.txt");
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

void day10()
{
    std::vector<std::string> display;
    int xReg = 1;
    int cycle = 0;
    int totalSignal = 0;

    auto clock = [&](int i, int nX)
    {
        const int pixel = i % 40;
        if (pixel == 0)
        {
            display.push_back("");
        }
        display.back() += abs(xReg - pixel) <= 1 ? '#' : ' ';
        if (pixel == 19)
        {
            totalSignal += (i + 1) * xReg;
        }
        xReg = nX;
    };

    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input10.txt");
    for (size_t i = 0; i < inputData.size(); i++)
    {
        std::istringstream lineStream(inputData[i]);
        std::string op;
        lineStream >> op;
        if (op == "noop")
        {
            clock(cycle++, xReg);
        }
        else if (op == "addx")
        {
            int val;
            lineStream >> val;
            clock(cycle++, xReg);
            clock(cycle++, xReg + val);
        }
    }

    std::cout << "AoC: Day 10: Part 1:  " << totalSignal << std::endl;
    std::cout << "AoC: Day 10: Part 2:" << std::endl;
    for (const std::string &line : display)
    {
        std::cout << line << std::endl;
    }
    std::cout << std::endl;
}

void day11()
{
    class Item
    {
    public:
        uint64_t m_worryLevel;
    };
    enum OpType
    {
        Adder = 0,
        Multiplier,
        Square
    };
    class Operation
    {
    public:
        OpType m_Type;
        int m_Val;

        void Apply(Item &item, bool bIsPart2)
        {
            switch (m_Type)
            {
            case OpType::Adder:
                item.m_worryLevel += m_Val;
                break;
            case OpType::Multiplier:
                item.m_worryLevel *= m_Val;
                break;
            case OpType::Square:
                item.m_worryLevel *= item.m_worryLevel;
                break;
            }
            if (!bIsPart2)
            {
                item.m_worryLevel /= 3;
            }
        }
    };
    class Monkey
    {
    public:
        std::vector<Item> m_Items;
        Operation m_Op;
        int m_Test;
        int m_MonkeyIfTrue;
        int m_MonkeyIfFalse;
        uint64_t m_ItemsInspected = 0;

        bool operator<(const Monkey &rhs) const
        {
            return m_ItemsInspected > rhs.m_ItemsInspected;
        }
    };
    std::vector<Monkey> MonkeysP1;
    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input11.txt");
    for (size_t i = 0; i < inputData.size(); i += 7)
    {
        std::string &monkey = inputData[i];
        std::string &items = inputData[i + 1];
        std::string &operation = inputData[i + 2];
        std::string &test = inputData[i + 3];
        std::string &ifTrue = inputData[i + 4];
        std::string &ifFalse = inputData[i + 5];
        MonkeysP1.push_back(Monkey());
        std::istringstream itemStream(items);
        itemStream.ignore(18);
        while (!itemStream.eof())
        {
            uint64_t worry = 0;
            itemStream >> worry;
            itemStream.ignore(2);
            MonkeysP1.back().m_Items.push_back(Item{worry});
        }
        std::istringstream operationStream(operation);
        operationStream.ignore(23);
        char op;
        std::string val;
        operationStream >> op;
        operationStream.ignore(1);
        operationStream >> val;
        if (val == "old")
        {
            MonkeysP1.back().m_Op.m_Type = OpType::Square;
        }
        else if (op == '*')
        {
            MonkeysP1.back().m_Op.m_Type = OpType::Multiplier;
            MonkeysP1.back().m_Op.m_Val = atoi(val.c_str());
        }
        else
        {
            MonkeysP1.back().m_Op.m_Type = OpType::Adder;
            MonkeysP1.back().m_Op.m_Val = atoi(val.c_str());
        }
        std::istringstream testStream(test);
        testStream.ignore(21);
        testStream >> MonkeysP1.back().m_Test;
        std::istringstream ifTrueStream(ifTrue);
        ifTrueStream.ignore(29);
        ifTrueStream >> MonkeysP1.back().m_MonkeyIfTrue;
        std::istringstream ifFalseStream(ifFalse);
        ifFalseStream.ignore(30);
        ifFalseStream >> MonkeysP1.back().m_MonkeyIfFalse;
    }
    std::vector<Monkey> MonkeysP2 = MonkeysP1;

    auto playRounds = [&](std::vector<Monkey> &monkeys, int numRounds, bool bIsPart2)
    {
        for (int round = 0; round < numRounds; round++)
        {
            for (size_t mIdx = 0; mIdx < monkeys.size(); mIdx++)
            {
                Monkey &monkey = monkeys[mIdx];
                for (size_t iIdx = 0; iIdx < monkey.m_Items.size(); iIdx++)
                {
                    Item &item = monkey.m_Items[iIdx];
                    monkey.m_Op.Apply(item, bIsPart2);
                    if (item.m_worryLevel % monkey.m_Test == 0)
                    {
                        monkeys[monkey.m_MonkeyIfTrue].m_Items.push_back(item);
                    }
                    else
                    {
                        monkeys[monkey.m_MonkeyIfFalse].m_Items.push_back(item);
                    }
                    monkey.m_ItemsInspected++;
                }
                monkey.m_Items.erase(monkey.m_Items.begin(), monkey.m_Items.end());
            }
        }
    };

    playRounds(MonkeysP1, 20, false);
    playRounds(MonkeysP2, 10000, true);

    std::sort(MonkeysP1.begin(), MonkeysP1.end());
    std::sort(MonkeysP2.begin(), MonkeysP2.end());
    std::cout << "AoC: Day 11: Part 1:  " << MonkeysP1[0].m_ItemsInspected * MonkeysP1[1].m_ItemsInspected << std::endl;
    // Part 2 incorrect due to massive integer overflow.
    std::cout << "AoC: Day 11: Part 2:  " << MonkeysP2[0].m_ItemsInspected * MonkeysP2[1].m_ItemsInspected << std::endl;
    std::cout << std::endl;
}

void day12()
{
    struct Loc
    {
    public:
        int x;
        int y;
        char terrain;
        int stepsAway = std::numeric_limits<int>::max();
    };
    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input12.txt");
    const int numCols = inputData[0].length();
    while (inputData.back().length() < numCols)
    {
        inputData.pop_back();
    }
    const int numRows = inputData.size();
    Loc *map = new Loc[numRows * numCols];
    int startX, startY;
    int endX, endY;
    auto getLoc = [&](int x, int y) -> Loc &
    { return map[x + (y * numCols)]; };
    std::vector<Loc *> lowestPoints;
    for (int y = 0; y < inputData.size(); y++)
    {
        std::string &line = inputData[y];
        for (int x = 0; x < line.length(); x++)
        {
            Loc &loc = getLoc(x, y) = Loc{x, y, line[x]};
            if (loc.terrain == 'S')
            {
                startX = x;
                startY = y;
                loc.terrain = 'a';
            }
            else if (loc.terrain == 'E')
            {
                endX = x;
                endY = y;
                loc.terrain = 'z';
            }
            if (loc.terrain == 'a')
            {
                lowestPoints.push_back(&loc);
            }
        }
    }
    std::vector<Loc *> processQueue;
    processQueue.push_back(&getLoc(endX, endY));
    processQueue.back()->stepsAway = 0;
    auto step = [&](Loc &to, Loc &from)
    {
        // Should no longer do this for part 2
        // if (startLoc->stepsAway <= from.stepsAway + 1)
        //{
        //    return;
        //}
        if (to.stepsAway <= from.stepsAway + 1)
        {
            return;
        }
        if (to.terrain < (from.terrain - 1))
        {
            return;
        }
        to.stepsAway = from.stepsAway + 1;
        processQueue.push_back(&to);
    };
    auto process = [&](Loc &loc)
    {
        if (loc.x < numCols)
        {
            step(getLoc(loc.x + 1, loc.y), loc);
        }
        if (loc.x > 0)
        {
            step(getLoc(loc.x - 1, loc.y), loc);
        }
        if (loc.y < numRows)
        {
            step(getLoc(loc.x, loc.y + 1), loc);
        }
        if (loc.y > 0)
        {
            step(getLoc(loc.x, loc.y - 1), loc);
        }
    };
    for (size_t i = 0; i < processQueue.size(); i++)
    {
        process(*processQueue[i]);
    }
    auto cmp = [&](const Loc *a, const Loc *b)
    {
        return a->stepsAway < b->stepsAway;
    };
    std::sort(lowestPoints.begin(), lowestPoints.end(), cmp);

    std::cout << "AoC: Day 12: Part 1:  " << getLoc(startX, startY).stepsAway << std::endl;
    std::cout << "AoC: Day 12: Part 2:  " << lowestPoints[0]->stepsAway << std::endl;
    std::cout << std::endl;
}

void day13()
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
            return Day13::Eval(lhs, rhs);
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

    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input13.txt");
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

void day14()
{
    // std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("input14.txt");
    // for (size_t i = 0; i < inputData.size(); i++)
    // {
    //     std::string &line = inputData[i];
    // }
    // std::cout << "AoC: Day 14: Part 1:  " << todo << std::endl;
    // std::cout << "AoC: Day 14: Part 2:  " << todo << std::endl;
    // std::cout << std::endl;
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
}

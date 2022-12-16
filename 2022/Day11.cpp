#include "Common.h"

void Day11()
{
    class Item
    {
    public:
        Item(int worry)
        {
            for (size_t i = 0; i < 10; i++)
            {
                m_worryLevel[i] = worry;
            }
        }
        int m_worryLevel[10] = {0};
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
            for (size_t i = 0; i < 10; i++)
            {
                switch (m_Type)
                {
                case OpType::Adder:
                    item.m_worryLevel[i] += m_Val;
                    break;
                case OpType::Multiplier:
                    item.m_worryLevel[i] *= m_Val;
                    break;
                case OpType::Square:
                    item.m_worryLevel[i] *= item.m_worryLevel[i];
                    break;
                }
                if (!bIsPart2)
                {
                    item.m_worryLevel[i] /= 3;
                }
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
    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("Data/input11.txt");
    int divisor = 1;
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
            MonkeysP1.back().m_Items.push_back(Item(worry));
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
        divisor *= MonkeysP1.back().m_Test;
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
            std::cout << "Round " << round << std::endl;

            for (size_t mIdx = 0; mIdx < monkeys.size(); mIdx++)
            {
                Monkey &monkey = monkeys[mIdx];
                for (size_t iIdx = 0; iIdx < monkey.m_Items.size(); iIdx++)
                {
                    Item &item = monkey.m_Items[iIdx];
                    for (size_t ugh = 0; ugh < monkeys.size(); ugh++)
                    {
                        item.m_worryLevel[ugh] = item.m_worryLevel[ugh] % monkeys[ugh].m_Test;
                        // item.m_worryLevel[ugh] = item.m_worryLevel[ugh] % divisor;
                    }
                    monkey.m_Op.Apply(item, bIsPart2);
                    if (item.m_worryLevel[mIdx] % monkey.m_Test == 0)
                    {
                        std::cout << " Monkey " << mIdx << ": true" << std::endl;
                        monkeys[monkey.m_MonkeyIfTrue].m_Items.push_back(item);
                    }
                    else
                    {
                        std::cout << " Monkey " << mIdx << ": false" << std::endl;
                        monkeys[monkey.m_MonkeyIfFalse].m_Items.push_back(item);
                    }
                    monkey.m_ItemsInspected++;
                }
                monkey.m_Items.erase(monkey.m_Items.begin(), monkey.m_Items.end());
            }

            for (size_t mIdx = 0; mIdx < monkeys.size(); mIdx++)
            {
                std::cout << " Monkey " << mIdx << ": " << monkeys[mIdx].m_Items.size() << "  " << monkeys[mIdx].m_ItemsInspected << std::endl;
            }
            std::cout << std::endl;
        }
    };

    playRounds(MonkeysP1, 20, false);
    // playRounds(MonkeysP2, 10000, true);

    std::sort(MonkeysP1.begin(), MonkeysP1.end());
    std::sort(MonkeysP2.begin(), MonkeysP2.end());
    std::cout << "AoC: Day 11: Part 1:  " << MonkeysP1[0].m_ItemsInspected * MonkeysP1[1].m_ItemsInspected << std::endl;
    // Part 2 incorrect due to massive integer overflow.
    std::cout << "AoC: Day 11: Part 2:  " << MonkeysP2[0].m_ItemsInspected * MonkeysP2[1].m_ItemsInspected << std::endl;
    std::cout << std::endl;
}

AoC::DaySetter DaySetter11(11, Day11);
#include "Common.h"

void Day07()
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

    std::vector<std::string> inputData = AoC::FileSystem::ReadAllLines("Data/input07.txt");
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

AoC::DaySetter DaySetter07(7, Day07);
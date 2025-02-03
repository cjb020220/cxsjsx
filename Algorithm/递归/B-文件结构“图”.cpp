#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <string.h>
#define N 30 + 5

class Directory
{
public:
    std::string name;
    std::vector<int> child;
    std::vector<std::string> file;
    ~Directory()
    {
        child.clear();
        file.clear();
    }
};

Directory *dir;
const std::string prefix = "|     ";
std::string str;
static int dataset = 0;
int count = 1;

void output(int root, int depth)
{
    if (!root)
        std::cout << "DATA SET " << ++dataset << ":\n";
    if (depth)
    {
        for (int i = 0; i < depth; ++i)
            std::cout << prefix;
    }
    std::cout << dir[root].name << '\n';
    for (auto iter : dir[root].child)
        output(iter, depth + 1);
    std::sort(dir[root].file.begin(), dir[root].file.end());
    for (auto iter : dir[root].file)
    {
        if (depth)
        {
            for (int j = 0; j < depth; ++j)
                std::cout << prefix;
        }
        std::cout << iter << '\n';
    }
    if (!root)
        std::cout << '\n';
}

void solve(int root)
{
    while (std::cin >> str)
    {
        if (str[0] == '#')
        {
            delete[] dir;
            return;
        }
        else if (str[0] == ']')
            return;
        else if (str[0] == '*')
        {
            output(0, 0);
            delete[] dir;
            dir = new Directory[N];
            dir[0].name = "ROOT";
            count = 1;
            continue;
        }
        else if (str[0] == 'f')
            dir[root].file.push_back(str);
        else
        {
            dir[root].child.push_back(count);
            dir[count].name = str;
            ++count;
            solve(count - 1);
        }
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    dir = new Directory[N];
    dir[0].name = "ROOT";
    solve(0);
}
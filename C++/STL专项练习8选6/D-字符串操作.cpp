#include <iostream>
#include <string>
#define N 20 + 5

std::string str[N];
std::string op;
int total;

std::string get_string();
int get_int();

bool check(std::string s1, std::string s2)
{
    int length1 = s1.size(), length2 = s2.size();
    if (length1 > 5 || length2 > 5 || !length1 || !length2)
        return false;
    for (int i = 0; i < length1; ++i)
    {
        if (s1[i] < '0' || s1[i] > '9')
            return false;
    }
    for (int i = 0; i < length2; ++i)
    {
        if (s2[i] < '0' || s2[i] > '9')
            return false;
    }
    int temp1 = std::stoi(s1), temp2 = std::stoi(s2);
    return temp1 >= 0 && temp1 <= 99999 && temp2 >= 0 && temp2 <= 99999;
}

std::string copy()
{
    int n = get_int(), x = get_int(), l = get_int();
    return str[n].substr(x, l);
}

std::string add()
{
    std::string s1 = get_string(), s2 = get_string();
    if (check(s1, s2))
        return std::to_string(std::stoi(s1) + std::stoi(s2));
    else
        return s1 + s2;
}

int find()
{
    std::string s = get_string();
    int n = get_int();
    int result = str[n].find(s);
    return result == std::string::npos ? str[n].size() : result;
}

int rfind()
{
    std::string s = get_string();
    int n = get_int();
    int result = str[n].rfind(s);
    return result == std::string::npos ? str[n].size() : result;
}

void insert()
{
    std::string s = get_string();
    int n = get_int(), x = get_int();
    str[n].insert(x, s);
}

void reset()
{
    std::string s = get_string();
    int n = get_int();
    str[n] = s;
}

void print()
{
    int n = get_int();
    std::cout << str[n] << '\n';
}

std::string get_string()
{
    std::string s;
    std::cin >> s;
    if (s == "copy")
        return copy();
    else if (s == "add")
        return add();
    else
        return s;
}

int get_int()
{
    std::string s;
    std::cin >> s;
    if (s == "find")
        return find();
    else if (s == "rfind")
        return rfind();
    else
        return std::stoi(s);
}

void solve()
{
    while (1)
    {
        std::cin >> op;
        if (op == "over")
            break;
        else if (op == "insert")
            insert();
        else if (op == "reset")
            reset();
        else if (op == "print")
            print();
        else if (op == "printall")
        {
            for (int i = 1; i <= total; ++i)
                std::cout << str[i] << '\n';
        }
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cin >> total;
    for (int i = 1; i <= total; ++i)
        std::cin >> str[i];
    solve();
}

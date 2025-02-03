#include <iostream>
#include <queue>
#include <unordered_map>
#include <math.h>

std::unordered_map<int, int> m;

int prime_factor(int x)
{
    if (m.find(x) != m.end())
        return m[x];
    int result = 0, sq = sqrt(x);
    int temp = x;
    for (int i = 2; i <= sq; ++i)
    {
        if (x % i == 0)
        {
            ++result;
            while (x % i == 0)
                x = x / i;
        }
    }
    if (x > 1 && result)
        ++result;
    m[temp] = result;
    return result;
}

class Myless
{
public:
    bool operator()(int x, int y)
    {
        int px = prime_factor(x), py = prime_factor(y);
        if (px == py)
            return x > y;
        return px > py;
    }
};

class Mygreater
{
public:
    bool operator()(int x, int y)
    {
        int px = prime_factor(x), py = prime_factor(y);
        if (px == py)
            return x < y;
        return px < py;
    }
};

std::priority_queue<int, std::vector<int>, Myless> less;
std::priority_queue<int, std::vector<int>, Mygreater> greater;
int n;

int main()
{
    int x;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            scanf("%d", &x);
            greater.push(x);
            less.push(x);
        }
        printf("%d %d\n", greater.top(), less.top());
        greater.pop();
        less.pop();
    }
}
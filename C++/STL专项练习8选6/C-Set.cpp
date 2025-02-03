#include <iostream>
#include <set>
#include <unordered_map>
#define LENGTH 3

std::multiset<int> s;
std::unordered_map<int, bool> m;
int n;

int main()
{
    int x;
    char op[LENGTH];
    scanf("%d", &n);
    for (int i = 0; i < n; ++i)
    {
        scanf("%s%d", op, &x);
        if (op[1] == 'd')
        {
            s.insert(x);
            printf("%d\n", s.count(x));
            m[x] = true;
        }
        else if (op[1] == 'e')
        {
            printf("%d\n", s.count(x));
            s.erase(x);
        }
        else
            printf("%d %d\n", m[x], s.count(x));
    }
}
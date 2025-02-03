#include <iostream>
#include <map>

std::map<int, int> m;
int n;

int main()
{
    int x, y;
    scanf("%d", &n);
    m[1000000000] = 1;
    for (int i = 0; i < n; ++i)
    {
        scanf("%d%d", &x, &y);
        auto iter = m.find(y);
        if (iter != m.end())
        {
            printf("%d %d\n", x, iter->second);
            if (x < m[y])
                m[y] = x;
            continue;
        }
        auto iter1 = m.lower_bound(y), iter2 = m.lower_bound(y);
        if (iter1 == m.begin())
            printf("%d %d\n", x, iter2->second);
        else
        {
            --iter1;
            if (iter2 == m.end())
                printf("%d %d\n", x, iter1->second);
            else
            {
                int left = y - iter1->first, right = iter2->first - y;
                if (left > right || (left == right && iter2->second < iter1->second))
                    printf("%d %d\n", x, iter2->second);
                else
                    printf("%d %d\n", x, iter1->second);
            }
        }
        m[y] = x;
    }
}
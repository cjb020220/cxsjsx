#include <iostream>
#include <algorithm>
#include <string.h>
#define N 100 + 5

class Point
{
public:
    int index, value;
    Point() {}
};

bool myless(const Point &p, const Point &q)
{
    return p.value < q.value;
}

bool mygreater(const Point &p, const Point &q)
{
    return p.value > q.value;
}

Point point[N];
int tree[N];
int k, n;
int result = 0;

int lowbit(int x)
{
    return x & -x;
}

void update(int x, int value)
{
    for (int i = x; i <= n; i += lowbit(i))
        tree[i] = std::max(tree[i], value);
}

int query(int x)
{
    int result1 = 0;
    for (int i = x; i; i -= lowbit(i))
        result1 = std::max(result1, tree[i]);
    return result1;
}

int main()
{
    scanf("%d", &k);
    while (k--)
    {
        scanf("%d", &n);
        memset(tree, 0, sizeof(tree));
        result = 0;
        for (int i = 1; i <= n; ++i)
        {
            point[i].index = i;
            scanf("%d", &point[i].value);
        }
        std::sort(point + 1, point + n + 1, myless);
        for (int i = 1; i <= n; ++i)
        {
            int temp = query(point[i].index) + 1;
            result = std::max(result, temp);
            update(point[i].index, temp);
        }
        memset(tree, 0, sizeof(tree));
        std::sort(point + 1, point + n + 1, mygreater);
        for (int i = 1; i <= n; ++i)
        {
            int temp = query(point[i].index) + 1;
            result = std::max(result, temp);
            update(point[i].index, temp);
        }
        printf("%d\n", result);
    }
}
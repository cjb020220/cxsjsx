#include <iostream>
#define N 50000

int value[N];
int l, n, m;

bool check(int x)
{
    int pos = 0, count = 0;
    for (int i = 0; i < n; ++i)
    {
        if (value[i] - pos < x)
            ++count;
        else
            pos = value[i];
    }
    if (l - pos < x)
        ++count;
    return count <= m;
}

void solve()
{
    int left = 1, right = l;
    while (right - left > 1)
    {
        int mid = (left + right) / 2;
        if (check(mid))
            left = mid;
        else
            right = mid;
    }
    printf("%d\n", left);
}

int main()
{
    scanf("%d%d%d", &l, &n, &m);
    for (int i = 0; i < n; ++i)
        scanf("%d", &value[i]);
    solve();
}
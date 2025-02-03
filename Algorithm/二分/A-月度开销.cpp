#include <iostream>
#define N 100000

int value[N];
int max_value = 0, total_value = 0;
int n, m;

bool check(int x)
{
    int sum = 0, count = 0;
    for (int i = 0; i < n; ++i)
    {
        sum += value[i];
        if (sum > x)
        {
            sum = value[i];
            ++count;
        }
    }
    return count + 1 <= m;
}

void solve()
{
    int left = max_value, right = total_value;
    while (left < right)
    {
        int mid = (left + right) / 2;
        if (check(mid))
            right = mid;
        else
            left = mid + 1;
    }
    printf("%d\n", left);
}

int main()
{
    scanf("%d%d", &n, &m);
    for (int i = 0; i < n; ++i)
    {
        scanf("%d", &value[i]);
        max_value = std::max(max_value, value[i]);
        total_value += value[i];
    }
    solve();
}
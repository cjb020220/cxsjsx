#include <iostream>
#include <vector>
#include <string.h>
#define N 200
#define SUM 10000 + 5

std::vector<int> result;
int dp[SUM], dp_remove[SUM];
int value[N];
int n, x;

int main()
{
    scanf("%d%d", &n, &x);
    for (int i = 0; i < n; ++i)
        scanf("%d", &value[i]);
    dp[0] = 1;
    dp[value[0]] = 1;
    for (int i = 1; i < n; ++i)
    {
        for (int j = x; j >= value[i]; --j)
            dp[j] += dp[j - value[i]];
    }
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j <= x; ++j)
        {
            if (j < value[i])
                dp_remove[j] = dp[j];
            else
                dp_remove[j] = dp[j] - dp_remove[j - value[i]];
        }
        if (!dp_remove[x])
            result.push_back(value[i]);
    }
    int count = result.size();
    printf("%d\n", count);
    for (int i = 0; i < count; ++i)
    {
        printf("%d", result[i]);
        if (i < count - 1)
            putchar(' ');
    }
    putchar('\n');
}
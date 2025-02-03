#include <iostream>
#define N 100

int dp[N];
int location[N], profit[N];
int n, k;
int result = 0;

int main()
{
    int t;
    scanf("%d", &t);
    while (t--)
    {
        scanf("%d%d", &n, &k);
        result = 0;
        for (int i = 0; i < n; ++i)
            scanf("%d", &location[i]);
        for (int i = 0; i < n; ++i)
            scanf("%d", &profit[i]);
        for (int i = 0; i < n; ++i)
        {
            dp[i] = profit[i];
            for (int j = 0; j < i; ++j)
            {
                if (location[i] - location[j] <= k)
                    break;
                dp[i] = std::max(dp[i], dp[j] + profit[i]);
            }
            result = std::max(result, dp[i]);
        }
        printf("%d\n", result);
    }
}
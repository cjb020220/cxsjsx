#include <iostream>

long long dp[18][3] = {0};
int n;
int count, num;

int main()
{
    scanf("%d", &n);
    for (int i = 0; i < n; ++i)
    {
        long long temp[3] = {0};
        scanf("%d", &count);
        for (int j = 0; j < count; ++j)
        {
            scanf("%d", &num);
            ++temp[num % 3];
        }
        if (!i)
        {
            dp[i][0] = temp[0];
            dp[i][1] = temp[1];
            dp[i][2] = temp[2];
        }
        else
        {
            dp[i][0] = dp[i - 1][0] * temp[0] + dp[i - 1][1] * temp[2] + dp[i - 1][2] * temp[1];
            dp[i][1] = dp[i - 1][0] * temp[1] + dp[i - 1][1] * temp[0] + dp[i - 1][2] * temp[2];
            dp[i][2] = dp[i - 1][0] * temp[2] + dp[i - 1][1] * temp[1] + dp[i - 1][2] * temp[0];
        }
    }
    printf("%lld\n", dp[n - 1][0]);
}
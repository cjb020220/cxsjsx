#include <iostream>
#define N 250

long long dp[N][N] = {0}, result[N] = {0};
int n;

void solve()
{
    for (int i = 1; i < N; ++i)
    {
        dp[i][i] = 1;
        result[i] = 1;
        for (int j = 1; j <= i / 2; ++j)
        {
            if (j * 2 == i)
                dp[i][j] = 1;
            for (int k = j; k <= i - 2 * j; ++k)
                dp[i][j] += dp[i - 2 * j][k];
            result[i] += dp[i][j];
        }
    }
}

int main()
{
    solve();
    while (scanf("%d", &n))
    {
        if (!n)
            break;
        printf("%d %lld\n", n, result[n]);
    }
}
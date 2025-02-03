#include <iostream>
#define N 3402
#define M 12880

int dp[M] = {0};
int weight[N], value[N];
int n, m;

int main()
{
    scanf("%d%d", &n, &m);
    for (int i = 0; i < n; ++i)
        scanf("%d%d", &weight[i], &value[i]);
    for (int i = weight[0]; i <= m; ++i)
        dp[i] = value[0];
    for (int i = 1; i < n; ++i)
    {
        for (int j = m; j >= weight[i]; --j)
            dp[j] = std::max(dp[j], dp[j - weight[i]] + value[i]);
    }
    printf("%d\n", dp[m]);
}
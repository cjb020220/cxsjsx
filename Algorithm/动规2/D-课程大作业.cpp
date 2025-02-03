#include <iostream>
#include <string.h>
#define N 15
#define LENGTH 50 + 5
#define INF 0x3f3f3f3f

class Point
{
public:
    int prev;
    int current;
    int penalty;
    int time;
};

Point dp[1 << N];
char name[N][LENGTH];
int deadline[N];
int required_time[N];
int order_prev[N], order_current[N], length;
int t, n;

int mycompare()
{
    for (int i = length - 1; i >= 0; --i)
    {
        if (order_current[i] < order_prev[i])
            return -1;
        else if (order_current[i] > order_prev[i])
            return 1;
    }
    return 0;
}

void solve()
{
    int root;
    dp[0].penalty = 0;
    dp[0].time = 0;
    for (int i = 1; i < (1 << n); ++i)
    {
        dp[i].penalty = INF;
        for (int j = 0; j < n; ++j)
        {
            if (i & (1 << j))
            {
                int prev_state = i ^ (1 << j);
                int temp = std::max(dp[prev_state].time + required_time[j] - deadline[j], 0);
                if (dp[prev_state].penalty + temp < dp[i].penalty)
                {
                    dp[i].prev = prev_state;
                    dp[i].current = j;
                    dp[i].penalty = dp[prev_state].penalty + temp;
                    dp[i].time = dp[prev_state].time + required_time[j];
                }
                else if (dp[prev_state].penalty + temp == dp[i].penalty)
                {
                    length = 0;
                    order_current[length++] = j;
                    root = prev_state;
                    while (root)
                    {
                        order_current[length++] = dp[root].current;
                        root = dp[root].prev;
                    }
                    length = 0;
                    root = i;
                    while (root)
                    {
                        order_prev[length++] = dp[root].current;
                        root = dp[root].prev;
                    }
                    if (mycompare() == -1)
                    {
                        dp[i].prev = prev_state;
                        dp[i].current = j;
                        dp[i].penalty = dp[prev_state].penalty + temp;
                        dp[i].time = dp[prev_state].time + required_time[j];
                    }
                }
            }
        }
    }
    printf("%d\n", dp[(1 << n) - 1].penalty);
    root = (1 << n) - 1;
    length = 0;
    while (root)
    {
        order_current[length++] = dp[root].current;
        root = dp[root].prev;
    }
    for (int i = n - 1; i >= 0; --i)
        printf("%s\n", name[order_current[i]]);
}

int main()
{
    scanf("%d", &t);
    while (t--)
    {
        scanf("%d", &n);
        memset(dp, 0, sizeof(dp));
        for (int i = 0; i < n; ++i)
            scanf("%s%d%d", name[i], &deadline[i], &required_time[i]);
        solve();
    }
}
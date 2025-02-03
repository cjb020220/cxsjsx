#include <iostream>
#include <string.h>
#define N 16
#define INF 0x3f3f3f3f

int edges[N][N];
int record[N][1 << N];
int result = INF;
int n;

void dfs(int u, int state, int cost)
{
    if (cost >= result || cost >= record[u][state])
        return;
    record[u][state] = cost;
    if (u == n - 1 && state == (1 << n) - 1)
    {
        result = std::min(result, cost);
        return;
    }
    for (int i = 0; i < n; ++i)
    {
        if (!(state & (1 << i)))
            dfs(i, state | (1 << i), cost + edges[u][i]);
    }
}

int main()
{
    scanf("%d", &n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            scanf("%d", &edges[i][j]);
    memset(record, 0x3f, sizeof(record));
    dfs(0, 1, 0);
    printf("%d\n", result);
}
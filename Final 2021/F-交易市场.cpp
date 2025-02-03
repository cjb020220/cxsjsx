#include <iostream>
#include <vector>
#include <queue>
#define N 1000
#define INF 1000000000

std::vector<std::pair<int, int>> edges[N];
std::queue<int> q;
bool state[N] = {0};
int update[N] = {0};
int dist[N] = {0};
int n, m;

int spfa()
{
    while (!q.empty())
    {
        int u = q.front();
        state[u] = false;
        for (auto iter : edges[u])
        {
            int v = iter.first;
            if (dist[u] + iter.second > dist[v])
            {
                dist[v] = dist[u] + iter.second;
                ++update[v];
                if (update[v] == n)
                    return INF;
                if (!state[v])
                {
                    state[v] = true;
                    q.push(v);
                }
            }
        }
        q.pop();
    }
    int result = 0;
    for (int i = 0; i < n; ++i)
        result = std::max(result, dist[i]);
    return result;
}

int main()
{
    int x, y, z;
    scanf("%d%d", &n, &m);
    for (int i = 0; i < m; ++i)
    {
        scanf("%d%d%d", &x, &y, &z);
        edges[x - 1].push_back(std::make_pair(y - 1, z));
    }
    q.push(0);
    state[0] = true;
    printf("%d\n", spfa());
}
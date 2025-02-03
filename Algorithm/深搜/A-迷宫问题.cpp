#include <iostream>
#define N 5
#define INF 0x3f3f3f3f

int board[N][N];
bool visited[N][N] = {0};
const int dx[4] = {-1, 1, 0, 0}, dy[4] = {0, 0, -1, 1};
std::pair<int, int> path[N * N], result[N * N];
int min_length = INF;

inline bool check(int x, int y)
{
    return x >= 0 && x < N && y >= 0 && y < N;
}

void dfs(int x, int y, int depth)
{
    if (x == 4 && y == 4)
    {
        if (depth < min_length)
        {
            min_length = depth;
            path[depth] = std::make_pair(x, y);
            for (int i = 0; i <= depth; ++i)
                result[i] = path[i];
        }
        return;
    }
    if (visited[x][y])
        return;
    if (depth >= min_length)
        return;
    visited[x][y] = true;
    path[depth] = std::make_pair(x, y);
    for (int i = 0; i < 4; ++i)
    {
        int new_x = x + dx[i], new_y = y + dy[i];
        if (!check(new_x, new_y) || board[new_x][new_y])
            continue;
        dfs(new_x, new_y, depth + 1);
    }
    visited[x][y] = false;
}

int main()
{
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            scanf("%d", &board[i][j]);
    dfs(0, 0, 0);
    for (int i = 0; i <= min_length; ++i)
        printf("(%d, %d)\n", result[i].first, result[i].second);
}
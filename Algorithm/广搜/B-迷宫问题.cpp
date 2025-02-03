#include <iostream>
#include <queue>
#define N 5

std::queue<std::pair<int, int>> q;
const int dx[4] = {-1, 1, 0, 0}, dy[4] = {0, 0, -1, 1};
int board[N][N];
bool visited[N][N];
std::pair<int, int> prev[N][N];

inline bool check(int x, int y)
{
    return x >= 0 && x < N && y >= 0 && y < N;
}

void bfs()
{
    while (!q.empty())
    {
        std::pair<int, int> p = q.front();
        if (!p.first && !p.second)
            break;
        for (int i = 0; i < 4; ++i)
        {
            int new_x = p.first + dx[i], new_y = p.second + dy[i];
            if (!check(new_x, new_y) || board[new_x][new_y])
                continue;
            if (visited[new_x][new_y])
                continue;
            prev[new_x][new_y] = p;
            visited[new_x][new_y] = true;
            q.push(std::make_pair(new_x, new_y));
        }
        q.pop();
    }
    int x = 0, y = 0;
    while (1)
    {
        printf("(%d, %d)\n", x, y);
        if (x == 4 && y == 4)
            break;
        std::pair<int, int> p = prev[x][y];
        x = p.first;
        y = p.second;
    }
}

int main()
{
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            scanf("%d", &board[i][j]);
    q.push(std::make_pair(4, 4));
    bfs();
}
#include <iostream>
#include <vector>
#include <queue>
#define N 300

class Point
{
public:
    int x, y, time;
    Point(int x_, int y_, int time_) : x(x_), y(y_), time(time_) {}
    bool operator<(const Point &p) const
    {
        return time > p.time;
    }
};

std::priority_queue<Point> q;
const int dx[4] = {-1, 1, 0, 0}, dy[4] = {0, 0, -1, 1};
int board[N][N];
bool visited[N][N] = {0};
int n;

bool check(int x, int y)
{
    return x >= 0 && x < n && y >= 0 && y < n;
}

void bfs()
{
    while (!q.empty())
    {
        Point p = q.top();
        q.pop();
        if (p.x == n - 1 && p.y == n - 1)
        {
            printf("%d\n", p.time);
            return;
        }
        for (int i = 0; i < 4; ++i)
        {
            int new_x = p.x + dx[i], new_y = p.y + dy[i];
            if (!check(new_x, new_y) || visited[new_x][new_y])
                continue;
            visited[new_x][new_y] = true;
            q.push(Point(new_x, new_y, std::max(p.time, board[new_x][new_y])));
        }
    }
}

int main()
{
    scanf("%d", &n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            scanf("%d", &board[i][j]);
    visited[0][0] = true;
    q.push(Point(0, 0, board[0][0]));
    bfs();
}
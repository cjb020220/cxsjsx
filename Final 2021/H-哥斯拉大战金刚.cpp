#include <iostream>
#include <vector>
#include <queue>
#define N 100
#define KEY 9 + 1
#define TRAP 7

class Point
{
public:
    int x, y, key, trap, time;
    Point(int x_, int y_, int key_, int trap_, int time_) : x(x_), y(y_), key(key_), trap(trap_), time(time_) {}
    bool operator<(const Point &p) const
    {
        return time > p.time;
    }
};

std::priority_queue<Point> q;
const int dx[4] = {-1, 1, 0, 0}, dy[4] = {0, 0, -1, 1};
char board[N][N];
bool visited[N][N][KEY][1 << TRAP] = {0};
int n, m;
int key_count = 0, trap_count = 0;

bool check(int x, int y)
{
    return x >= 0 && x < n && y >= 0 && y < m;
}

void bfs()
{
    while (!q.empty())
    {
        Point p = q.top();
        q.pop();
        if (board[p.x][p.y] == 'T' && p.key == key_count)
        {
            printf("%d\n", p.time);
            return;
        }
        for (int i = 0; i < 4; ++i)
        {
            int new_x = p.x + dx[i], new_y = p.y + dy[i];
            if (!check(new_x, new_y) || board[new_x][new_y] == 'X')
                continue;
            else if (board[new_x][new_y] >= '1' && board[new_x][new_y] <= '9')
            {
                int temp = board[new_x][new_y] - '0';
                if (p.key + 1 == temp)
                {
                    if (!visited[new_x][new_y][p.key + 1][p.trap])
                    {
                        visited[new_x][new_y][p.key + 1][p.trap] = true;
                        q.push(Point(new_x, new_y, p.key + 1, p.trap, p.time + 1));
                    }
                }
                else
                {
                    if (!visited[new_x][new_y][p.key][p.trap])
                    {
                        visited[new_x][new_y][p.key][p.trap] = true;
                        q.push(Point(new_x, new_y, p.key, p.trap, p.time + 1));
                    }
                }
            }
            else if (board[new_x][new_y] >= 'A' && board[new_x][new_y] <= 'G')
            {
                int temp = board[new_x][new_y] - 'A';
                if (p.trap & (1 << temp))
                {
                    if (!visited[new_x][new_y][p.key][p.trap])
                    {
                        visited[new_x][new_y][p.key][p.trap] = true;
                        q.push(Point(new_x, new_y, p.key, p.trap, p.time + 1));
                    }
                }
                else
                {
                    if (!visited[new_x][new_y][p.key][p.trap | (1 << temp)])
                    {
                        visited[new_x][new_y][p.key][p.trap | (1 << temp)] = true;
                        q.push(Point(new_x, new_y, p.key, p.trap | (1 << temp), p.time + 2));
                    }
                }
            }
            else
            {
                if (!visited[new_x][new_y][p.key][p.trap])
                {
                    visited[new_x][new_y][p.key][p.trap] = true;
                    q.push(Point(new_x, new_y, p.key, p.trap, p.time + 1));
                }
            }
        }
    }
    return;
}

int main()
{
    scanf("%d%d", &n, &m);
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            std::cin >> board[i][j];
            if (board[i][j] == 'S')
            {
                q.push(Point(i, j, 0, 0, 0));
                visited[i][j][0][0] = true;
            }
            else if (board[i][j] == 'G')
            {
                board[i][j] = 'A' + trap_count;
                ++trap_count;
            }
            else if (board[i][j] >= '1' && board[i][j] <= '9')
                key_count = std::max(key_count, board[i][j] - '0');
        }
    }
    bfs();
}

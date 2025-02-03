#include <iostream>
#include <vector>
#include <queue>
#include <string.h>
#define N 100
#define KEY 9 + 1
#define SNAKE 5

class Point
{
public:
    int x, y, key, snake, time;
    Point(int x_, int y_, int key_, int snake_, int time_) : x(x_), y(y_), key(key_), snake(snake_), time(time_) {}
    bool operator<(const Point &p) const
    {
        return time > p.time;
    }
};

std::priority_queue<Point> q;
const int dx[4] = {-1, 1, 0, 0}, dy[4] = {0, 0, -1, 1};
bool visited[N][N][KEY][1 << SNAKE];
char board[N][N];
int n;
int key, snake;

inline bool check(int x, int y)
{
    return x >= 0 && x < n && y >= 0 && y < n;
}

void bfs()
{
    while (!q.empty())
    {
        Point p = q.top();
        q.pop();
        if (board[p.x][p.y] == 'T' && p.key == key)
        {
            printf("%d\n", p.time);
            return;
        }
        for (int i = 0; i < 4; ++i)
        {
            int new_x = p.x + dx[i], new_y = p.y + dy[i];
            if (!check(new_x, new_y))
                continue;
            char c = board[new_x][new_y];
            if (c == '#')
                continue;
            else if (c >= '1' && c <= '9')
            {
                if (c - '0' == p.key + 1)
                {
                    if (!visited[new_x][new_y][p.key + 1][p.snake])
                    {
                        visited[new_x][new_y][p.key + 1][p.snake] = true;
                        q.push(Point(new_x, new_y, p.key + 1, p.snake, p.time + 1));
                    }
                }
                else
                {
                    if (!visited[new_x][new_y][p.key][p.snake])
                    {
                        visited[new_x][new_y][p.key][p.snake] = true;
                        q.push(Point(new_x, new_y, p.key, p.snake, p.time + 1));
                    }
                }
            }
            else if (c >= 'A' && c <= 'E')
            {
                if (!(p.snake & (1 << (c - 'A'))))
                {
                    if (!visited[new_x][new_y][p.key][p.snake | (1 << (c - 'A'))])
                    {
                        visited[new_x][new_y][p.key][p.snake | (1 << (c - 'A'))] = true;
                        q.push(Point(new_x, new_y, p.key, p.snake | (1 << (c - 'A')), p.time + 2));
                    }
                }
                else
                {
                    if (!visited[new_x][new_y][p.key][p.snake])
                    {
                        visited[new_x][new_y][p.key][p.snake] = true;
                        q.push(Point(new_x, new_y, p.key, p.snake, p.time + 1));
                    }
                }
            }
            else
            {
                if (!visited[new_x][new_y][p.key][p.snake])
                {
                    visited[new_x][new_y][p.key][p.snake] = true;
                    q.push(Point(new_x, new_y, p.key, p.snake, p.time + 1));
                }
            }
        }
    }
    puts("impossible");
}

int main()
{
    while (scanf("%d%d", &n, &key))
    {
        if (!n && !key)
            break;
        while (!q.empty())
            q.pop();
        memset(visited, 0, sizeof(visited));
        snake = 0;
        getchar();
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                board[i][j] = getchar();
                if (board[i][j] == 'K')
                {
                    q.push(Point(i, j, 0, 0, 0));
                    visited[i][j][0][0] = true;
                }
                else if (board[i][j] == 'S')
                {
                    board[i][j] = 'A' + snake;
                    ++snake;
                }
            }
            getchar();
        }
        bfs();
    }
}
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#define N 100

class Point
{
public:
    int x, y, height;
    Point(int x_, int y_, int height_) : x(x_), y(y_), height(height_) {}
    bool operator<(const Point &p) const
    {
        return height < p.height;
    }
};

std::priority_queue<Point> q;
const int dx[4] = {-1, 1, 0, 0}, dy[4] = {0, 0, -1, 1};
int dp[N][N];
int height[N][N];
int r, c;
int result = 0;

inline bool check(int x, int y)
{
    return x >= 0 && x < r && y >= 0 && y < c;
}

int main()
{
    scanf("%d%d", &r, &c);
    for (int i = 0; i < r; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            scanf("%d", &height[i][j]);
            q.push(Point(i, j, height[i][j]));
            dp[i][j] = 1;
        }
    }
    while (!q.empty())
    {
        Point p = q.top();
        for (int i = 0; i < 4; ++i)
        {
            int new_x = p.x + dx[i], new_y = p.y + dy[i];
            if (check(new_x, new_y) && height[new_x][new_y] > height[p.x][p.y])
                dp[p.x][p.y] = std::max(dp[p.x][p.y], dp[new_x][new_y] + 1);
            result = std::max(result, dp[p.x][p.y]);
        }
        q.pop();
    }
    printf("%d\n", result);
}
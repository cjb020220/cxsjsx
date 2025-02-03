#include <iostream>

int board[9][9];
int t;

bool check(int x, int y, int value)
{
    for (int i = 0; i < 9; ++i)
    {
        if (board[x][i] == value || board[i][y] == value)
            return false;
    }
    int start_x = 3 * (x / 3), start_y = 3 * (y / 3);
    for (int i = start_x; i <= start_x + 2; ++i)
    {
        for (int j = start_y; j <= start_y + 2; ++j)
        {
            if (board[i][j] == value)
                return false;
        }
    }
    return true;
}

bool dfs(int x, int y)
{
    if (x == 9)
    {
        for (int i = 0; i < 9; ++i)
        {
            for (int j = 0; j < 9; ++j)
                printf("%d", board[i][j]);
            putchar('\n');
        }
        return true;
    }
    int new_x = x, new_y = y + 1;
    if (y == 8)
    {
        new_x = x + 1;
        new_y = 0;
    }
    if (board[x][y])
        return dfs(new_x, new_y);
    bool flag = true;
    for (int k = 1; k <= 9; ++k)
    {
        if (check(x, y, k))
        {
            board[x][y] = k;
            if (dfs(new_x, new_y))
                return true;
            board[x][y] = 0;
        }
    }
    return false;
}

int main()
{
    scanf("%d", &t);
    while (t--)
    {
        for (int i = 0; i < 9; ++i)
        {
            getchar();
            for (int j = 0; j < 9; ++j)
            {
                char c = getchar();
                board[i][j] = c - '0';
            }
        }
        dfs(0, 0);
    }
}
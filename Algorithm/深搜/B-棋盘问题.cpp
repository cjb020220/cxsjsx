#include <iostream>
#define N 8

int board[N][N];
int n, k;

inline bool check(int column)
{
    for (int i = 0; i < n; ++i)
    {
        if (board[i][column] == 1)
            return false;
    }
    return true;
}

int dfs(int row, int count)
{
    if (row == n || !count)
        return 1;
    int result = 0;
    for (int i = 0; i < n; ++i)
    {
        if (!board[row][i] && check(i))
        {
            board[row][i] = 1;
            result += dfs(row + 1, count - 1);
            board[row][i] = 0;
        }
    }
    if (n - row > count)
        result += dfs(row + 1, count);
    return result;
}

int main()
{
    char c;
    while (scanf("%d%d", &n, &k))
    {
        if (n == -1 && k == -1)
            break;
        getchar();
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                c = getchar();
                if (c == '.')
                    board[i][j] = -1;
                else
                    board[i][j] = 0;
            }
            getchar();
        }
        printf("%d\n", dfs(0, k));
    }
}
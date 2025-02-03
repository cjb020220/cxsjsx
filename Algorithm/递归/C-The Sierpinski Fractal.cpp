#include <iostream>
#include <string.h>
#define N 1 << 10

char board[N][2 * N];
int n;

void solve(int n, int x, int y)
{
    if (n == 1)
    {
        board[x][y + 1] = '/';
        board[x][y + 2] = '\\';
        board[x + 1][y] = '/';
        board[x + 1][y + 1] = '_';
        board[x + 1][y + 2] = '_';
        board[x + 1][y + 3] = '\\';
        return;
    }
    solve(n - 1, x, y + (1 << (n - 1)));
    solve(n - 1, x + (1 << (n - 1)), y);
    solve(n - 1, x + (1 << (n - 1)), y + (1 << n));
}

int main()
{
    while (scanf("%d", &n))
    {
        if (!n)
            break;
        memset(board, 0, sizeof(board));
        solve(n, 0, 0);
        for (int j = 0; j < (1 << n); ++j)
        {
            for (int k = 0; k < (1 << (n + 1)); ++k)
            {
                if (!board[j][k])
                    putchar(' ');
                else
                    putchar(board[j][k]);
            }
            putchar('\n');
        }
        putchar('\n');
    }
}
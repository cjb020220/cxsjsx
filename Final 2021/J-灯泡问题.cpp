#include <iostream>
#include <bitset>
#include <string.h>
#define N 15
#define M 100

bool light[N][M], light_copy[N][M];
int count[M];
int n, m;
int result = 0;
std::bitset<N> bit;

int solve(int x)
{
    int result = 0;
    memset(count, 0, sizeof(count));
    bit = x;
    for (int i = 0; i < n; ++i)
    {
        if (bit[i])
        {
            for (int j = 0; j < m; ++j)
            {
                light_copy[i][j] = !light[i][j];
                count[j] += light_copy[i][j];
            }
        }
        else
        {
            for (int j = 0; j < m; ++j)
            {
                light_copy[i][j] = light[i][j];
                count[j] += light_copy[i][j];
            }
        }
    }
    for (int i = 0; i < m; ++i)
        result += std::max(count[i], n - count[i]);
    return result;
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--)
    {
        scanf("%d%d", &n, &m);
        result = 0;
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j)
                scanf("%d", &light[i][j]);
        for (int i = 0; i < (1 << n); ++i)
            result = std::max(result, solve(i));
        printf("%d\n", result);
    }
}
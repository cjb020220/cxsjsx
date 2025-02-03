#include <iostream>
#include <string.h>
#include <bitset>
#define N 29
#define INF 0x3f3f3f3f

int bit;
std::bitset<N> src, dest;
std::bitset<N> src_copy;
int n;
int result = INF;

void solve(int x)
{
    int temp = 0;
    bit = x;
    src_copy = src;
    for (int i = 0; i < n; ++i)
    {
        temp += bit;
        if (bit)
        {
            src_copy.flip(i);
            if (i)
                src_copy.flip(i - 1);
            if (i < n - 1)
                src_copy.flip(i + 1);
        }
        bit = (src_copy[i] != dest[i]);
    }
    if (src_copy[n - 1] == dest[n - 1])
        result = std::min(result, temp);
}

int main()
{
    char c;
    while (c = getchar())
    {
        if (c == '\n' || c == EOF)
            break;
        src[n++] = c - '0';
    }
    n = 0;
    while (c = getchar())
    {
        if (c == '\n' || c == EOF)
            break;
        dest[n++] = c - '0';
    }
    for (int i = 0; i < 2; ++i)
        solve(i);
    if (result == INF)
        puts("impossible");
    else
        printf("%d\n", result);
}
#include <iostream>
#include <string.h>
#define N 9
#define INF 0x3f3f3f3f

const char move[N][N] = {"0134", "012", "1245", "036", "13457", "258", "3467", "678", "4578"};
const int length[N] = {4, 3, 4, 3, 5, 3, 4, 3, 4};
int state[N];
int state_copy[N];
int move_times[N];
int min_length = INF;
int result[N];

void solve(int x)
{
    int total = 0;
    memcpy(state_copy, state, sizeof(int) * N);
    for (int i = 0; i < N; ++i)
    {
        move_times[i] = x >> (2 * i) & 3;
        total += move_times[i];
    }
    for (int i = 0; i < N; ++i)
    {
        if (move_times[i])
        {
            for (int j = 0; j < length[i]; ++j)
            {
                int temp = move[i][j] - '0';
                state_copy[temp] = (state_copy[temp] + move_times[i]) % 4;
            }
        }
    }
    for (int i = 0; i < N; ++i)
    {
        if (state_copy[i])
            return;
    }
    if (total < min_length)
    {
        min_length = total;
        memcpy(result, move_times, sizeof(int) * N);
    }
}

int main()
{
    for (int i = 0; i < N; ++i)
        scanf("%d", &state[i]);
    for (int i = 0; i < (1 << 2 * N); ++i)
        solve(i);
    for (int i = 0; i < N; ++i)
    {
        while (result[i])
        {
            putchar(i + 1 + '0');
            --result[i];
            --min_length;
            if (min_length)
                putchar(' ');
            else
                putchar('\n');
        }
    }
}
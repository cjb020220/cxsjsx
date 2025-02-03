#include <iostream>
#include <algorithm>
#define N 1000

int tian[N], king[N];
int n;
int result;

int solve()
{
    int result = 0;
    int tian_left = 0, tian_right = n - 1;
    int king_left = 0, king_right = n - 1;
    for (int i = 0; i < n; ++i)
    {
        if (tian[tian_left] > king[king_left])
        {
            ++tian_left;
            ++king_left;
            result += 200;
        }
        else if (tian[tian_left] < king[king_left])
        {
            ++tian_left;
            --king_right;
            result -= 200;
        }
        else
        {
            if (tian[tian_right] > king[king_right])
            {
                --tian_right;
                --king_right;
                result += 200;
            }
            else if (tian[tian_right] < king[king_right] || (tian[tian_right] == king[king_right] && tian[tian_left] < king[king_right]))
            {
                ++tian_left;
                --king_right;
                result -= 200;
            }
            else
                break;
        }
    }
    return result;
}

int main()
{
    while (scanf("%d", &n))
    {
        if (!n)
            break;
        for (int i = 0; i < n; ++i)
            scanf("%d", &tian[i]);
        for (int i = 0; i < n; ++i)
            scanf("%d", &king[i]);
        std::sort(tian, tian + n);
        std::sort(king, king + n);
        printf("%d\n", solve());
    }
}
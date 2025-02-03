#include <iostream>
#include <string>
#define N 10000

int n, k;
int length[N];

void solve()
{
    int left = 1, right = 1e7;
    while (right - left > 1)
    {
        int mid = (left + right) / 2;
        long long temp = 0;
        for (int i = 0; i < n; ++i)
            temp += length[i] / mid;
        if (temp < k)
            right = mid;
        else
            left = mid;
    }
    long long temp = 0;
    for (int i = 0; i < n; ++i)
        temp += length[i] / (left + 1);
    if (temp == k)
        ++left;
    printf("%d.", left / 100);
    left = left % 100;
    if (left < 10)
        putchar('0');
    printf("%d\n", left);
}

int main()
{
    long long total = 0;
    scanf("%d%d", &n, &k);
    getchar();
    for (int i = 0; i < n; ++i)
    {
        std::string str = "";
        char c;
        while (1)
        {
            c = getchar();
            if (c == '\n')
                break;
            else if (c >= '0' && c <= '9')
                str += c;
        }
        length[i] = stoi(str);
        total += length[i];
    }
    if (total < k)
        puts("0.00");
    else
        solve();
}
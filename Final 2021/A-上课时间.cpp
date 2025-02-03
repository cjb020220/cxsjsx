#include <iostream>
#define N 1000

int start[N], end[N];
int n;
int result = 0;

int main()
{
    scanf("%d", &n);
    for (int i = 0; i < n; ++i)
        scanf("%d%d", &start[i], &end[i]);
    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            int temp1 = std::max(start[i], start[j]);
            int temp2 = std::min(end[i], end[j]);
            result = std::max(result, temp2 - temp1 + 1);
        }
    }
    printf("%d\n", result);
}
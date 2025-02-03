#include <iostream>
#define N 1000

int value[N];
int max_value = 0, sum = 0;
int n;

int main()
{
    while (scanf("%d", &n) != EOF)
    {
        max_value = 0;
        sum = 0;
        for (int i = 0; i < n; ++i)
        {
            scanf("%d", &value[i]);
            max_value = std::max(max_value, value[i]);
            sum += value[i];
        }
        if (max_value > sum - max_value)
            printf("%d.0\n", sum - max_value);
        else
            printf("%d.%d\n", sum / 2, 5 * (sum % 2));
    }
}
#include <iostream>
#include <math.h>
#define N 10000
#define PI acos(-1)
#define EPS 1e-6

double volume[N];
double max_volume = 0;
int n, f;

bool check(double x)
{
    int count = 0;
    for (int i = 0; i < n; ++i)
        count += (int)(volume[i] / x);
    return count > f;
}

void solve()
{
    double left = 0, right = max_volume;
    while (right - left >= EPS)
    {
        double mid = (left + right) / 2;
        if (check(mid))
            left = mid;
        else
            right = mid;
    }
    printf("%.3lf\n", left);
}

int main()
{
    int r;
    scanf("%d%d", &n, &f);
    for (int i = 0; i < n; ++i)
    {
        scanf("%d", &r);
        volume[i] = PI * r * r;
        max_volume = std::max(max_volume, volume[i]);
    }
    solve();
}
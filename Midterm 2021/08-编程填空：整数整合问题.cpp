#include <iostream>
using namespace std;
unsigned int calc(unsigned int n, unsigned m, int x, int y)
{
    unsigned int mask1 = x == 32 ? 0xffffffff : (1 << x) - 1;
    unsigned int mask2 = !y ? 0 : ~((1 << (32 - y)) - 1);
    unsigned int mask = mask1 | mask2;
    return (mask & n) | (~mask & m);
}
int main()
{
    int x, y, t;
    unsigned int n, m;
    cin >> t;
    while (t--)
    {
        cin >> n >> m >> x >> y;
        cout << calc(n, m, x, y) << endl;
    }
    return 0;
}
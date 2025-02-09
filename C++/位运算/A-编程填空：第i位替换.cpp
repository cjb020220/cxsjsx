#include <iostream>
using namespace std;

int bitManipulation1(int n, int m, int i)
{
    return ((1 << i) & m) + ((~(1 << i)) & n);
}

int main()
{
    int n, m, i, t;
    cin >> t;
    while (t--)
    {
        cin >> n >> m >> i;
        cout << bitManipulation1(n, m, i) << endl;
    }
    return 0;
}
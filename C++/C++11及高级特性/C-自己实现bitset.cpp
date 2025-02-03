#include <iostream>
#include <cstring>
using namespace std;
template <int bitNum>
struct MyBitset
{
    char a[bitNum / 8 + 1];
    MyBitset() { memset(a, 0, sizeof(a)); };
    void Set(int i, int v)
    {
        char &c = a[i / 8];
        int bp = i % 8;
        if (v)
            c |= (1 << bp);
        else
            c &= ~(1 << bp);
    }
    class Bit
    {
    public:
        int i;
        MyBitset &b;
        Bit(int i_, MyBitset &b_) : i(i_), b(b_) {}
        operator int() const
        {
            char c = b.a[i / 8];
            int bp = i % 8;
            return (c >> bp) & 1;
        }
        Bit &operator=(int v)
        {
            b.Set(i, v);
            return *this;
        }
        Bit &operator=(const Bit &bit)
        {
            b.Set(i, bit);
            return *this;
        }
    };
    Bit operator[](int i)
    {
        return Bit(i, *this);
    }
    void Print()
    {
        for (int i = 0; i < bitNum; ++i)
            cout << (*this)[i];
        cout << endl;
    }
};

int main()
{
    int n;
    int i, j, k, v;
    while (cin >> n)
    {
        MyBitset<20> bs;
        for (int i = 0; i < n; ++i)
        {
            int t;
            cin >> t;
            bs.Set(t, 1);
        }
        bs.Print();
        cin >> i >> j >> k >> v;
        bs[k] = v;
        bs[i] = bs[j] = bs[k];
        bs.Print();
        cin >> i >> j >> k >> v;
        bs[k] = v;
        (bs[i] = bs[j]) = bs[k];
        bs.Print();
    }
    return 0;
}
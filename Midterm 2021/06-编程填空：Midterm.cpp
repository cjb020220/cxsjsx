#include <iostream>
using namespace std;

class Midterm
{
private:
    int val;

public:
    Midterm() : val(0) {}
    Midterm(int val_) : val(val_) {}
    Midterm &operator-=(int n)
    {
        val -= n;
        return *this;
    }
    Midterm operator++(int)
    {
        ++val;
        return *this;
    }
    operator int()
    {
        return val;
    }
    friend ostream &operator<<(ostream &os, const Midterm &mid)
    {
        os << mid.val;
        return os;
    }
};

int Min(int a, int b)
{
    if (a < b)
        return a;
    else
        return b;
}
int main()
{
    Midterm a;
    cout << a << endl;
    cout << a + 2021 << endl;
    Midterm b(1000);
    cout << b++ << endl;
    ((b -= 10) -= 20) -= 30;
    cout << b++ << endl;
    cout << Min(255, b) << endl;
    return 0;
}
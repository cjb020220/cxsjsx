#include <cstdio>
#include <iostream>
using namespace std;

class f
{
private:
    int num1, num2;

public:
    f(int num_) : num1(num_) {}
    f(int num1_, int num2_) : num1(num1_), num2(num2_) {}
    int operator()(int n)
    {
        return n - num1;
    }
    operator int()
    {
        return num1 * num2;
    }
};

int main()
{
    cout << f(3)(5) << endl;
    cout << f(4)(10) << endl;
    cout << f(114)(514) << endl;
    cout << f(9, 7) << endl;
    cout << f(2, 3) << endl;
    cout << f(2, 5) << endl;
}
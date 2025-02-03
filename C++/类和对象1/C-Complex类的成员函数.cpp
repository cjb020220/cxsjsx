#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;
class Complex
{
private:
    double r, i;

public:
    void Print()
    {
        cout << r << "+" << i << "i" << endl;
    }
    Complex() {}
    Complex(const char *str) : r(str[0] - '0'), i(str[2] - '0') {}
};
int main()
{
    Complex a;
    a = "3+4i";
    a.Print();
    a = "5+6i";
    a.Print();
    return 0;
}
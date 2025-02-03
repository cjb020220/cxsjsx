#include <iostream>
using namespace std;
template <class F1, class F2, class T>
auto combine(F1 f, F2 g)
{
    return [=](T x)
    { return f(f(x) + g(x)); };
}
int main()
{
    auto Square = [](double a)
    { return a * a; };
    auto Inc = [](double a)
    { return a + 1; };
    cout << combine<decltype(Square), decltype(Inc), int>(Square, Inc)(3) << endl;
    cout << combine<decltype(Inc), decltype(Square), double>(Inc, Square)(2.5) << endl;

    return 0;
}
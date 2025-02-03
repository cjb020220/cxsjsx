#include <iostream>
using namespace std;
template <class T1, class T2>
auto f(T1 x)
{
    return [=](T2 y)
    { return y + x; };
}
int main()
{
    cout << f<int, int>(7)(9) << endl;                     // 16
    cout << f<string, string>(" hello!")("world") << endl; // world hello!
    cout << f<char, string>('!')("world") << endl;

    return 0; // world!
}
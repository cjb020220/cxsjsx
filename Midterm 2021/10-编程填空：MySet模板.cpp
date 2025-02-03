#include <iostream>
#include <map>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <cstdio>
using namespace std;
template <class T>
class MySet : public set<T, greater<T>>
{
public:
    template <class T1>
    void erase(T1 *start, T1 *end)
    {
        for (auto iter = start; iter != end; ++iter)
        {
            set<T, greater<T>>::erase(*iter);
        }
    }
    template <class T1>
    void erase(T1 iter)
    {
        set<T, greater<T>>::erase(iter);
    }
};
struct A
{
    int n;
    A(int i) : n(i) {}
    operator int()
    {
        return n;
    }
    bool operator>(const A &a) const
    {
        return n > a.n;
    }
};
template <class IT>
void print(IT s, IT e)
{
    for (; s != e; ++s)
        cout << *s << ",";
    cout << endl;
}
int main()
{
    MySet<int> mst;
    for (int i = 0; i < 10; ++i)
        mst.insert(i);
    print(mst.begin(), mst.end());
    int a[] = {1, 2, 3, 4};
    mst.erase(a, a + 3);
    print(mst.begin(), mst.end());

    A b[] = {{7}, {8}};
    mst.erase(b, b + 2);
    print(mst.begin(), mst.end());
    mst.erase(mst.find(6));
    print(mst.begin(), mst.end());
    for (int i = 0; i < 1000000; ++i)
        mst.insert(i);
    for (int i = 0; i < 1000000 - 10; ++i)
        mst.erase(mst.find(i));
    int n;
    cin >> n;
    MySet<A> msta;

    for (int i = 0; i < n; ++i)
    {
        string cmd;
        int n;
        cin >> cmd >> n;
        if (cmd == "ADD")
            msta.insert(A(n));
        else
        {
            if (msta.find(A(n)) != msta.end())
                cout << "YES" << endl;
            else
                cout << "NO" << endl;
        }
    }
    return 0;
}
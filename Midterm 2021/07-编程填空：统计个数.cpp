#include <iostream>
#include <string>
#include <set>
using namespace std;

class Counter
{
    int cnt;

public:
    Counter() : cnt(0) {}
    virtual void push(void *) = 0;
    virtual string name() = 0;
    void inc()
    {
        ++cnt;
    }
    int number()
    {
        return cnt;
    }
};

template <class T>
class TCounter : public Counter
{
public:
    string str;
    set<T> s;
    TCounter(string str_) : str(str_) {}
    virtual string name()
    {
        return str;
    }
    void push(void *x)
    {
        T *value = (T *)x;
        if (s.find(*value) == s.end())
        {
            inc();
            s.insert(*value);
        }
    }
};

Counter *build_counter(string name)
{
    if (name == "int")
        return new TCounter<int>("int");
    else
        return new TCounter<string>("string");
}

int main()
{
    int n, m;
    cin >> n >> m;
    Counter *a = build_counter("int"), *b = build_counter("string");
    for (int i = 0; i < n; ++i)
    {
        int x;
        cin >> x;
        a->push(&x);
    }
    for (int i = 0; i < m; ++i)
    {
        string x;
        cin >> x;
        b->push(&x);
    }
    cout << a->name() << ": " << a->number() << endl;
    cout << b->name() << ": " << b->number();
    return 0;
}
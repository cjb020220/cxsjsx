#include <iostream>
#include <list>
#include <string>
using namespace std;

template <class T1, class T2>
void Copy(T1 s, T1 e, T2 x)
{
    for (; s != e; ++s, ++x)
        *x = *s;
}

template <class T>
class myostream_iteraotr
{
private:
    char *sep;
    ostream &os;

public:
    myostream_iteraotr(ostream &os_, const char *sep_) : os(os_)
    {
        if (sep_)
        {
            int length = 0;
            while (sep_[length])
                ++length;
            sep = new char[length];
            for (int i = 0; sep_[i]; ++i)
                sep[i] = sep_[i];
            sep[length] = '\0';
        }
        else
            sep = NULL;
    }
    myostream_iteraotr &operator*()
    {
        return *this;
    }
    myostream_iteraotr &operator=(T value)
    {
        os << value << sep;
        return *this;
    }
    myostream_iteraotr &operator++()
    {
        return *this;
    }
};

int main()
{
    const int SIZE = 5;
    int a[SIZE] = {5, 21, 14, 2, 3};
    double b[SIZE] = {1.4, 5.56, 3.2, 98.3, 3.3};
    list<int> lst(a, a + SIZE);
    myostream_iteraotr<int> output(cout, ",");
    Copy(lst.begin(), lst.end(), output);
    cout << endl;
    myostream_iteraotr<double> output2(cout, "--");
    Copy(b, b + SIZE, output2);
    return 0;
}

#include <iostream>
#include <string>

using namespace std;
template <class T>
class CMyistream_iterator
{
private:
    T *value;
    istream &in;

public:
    CMyistream_iterator(istream &in_) : in(in_)
    {
        value = NULL;
    }
    T operator*()
    {
        if (!value)
        {
            value = new T;
            in >> *value;
        }
        return *value;
    }
    CMyistream_iterator operator++(int)
    {
        if (value)
        {
            delete value;
            value = nullptr;
        }
        return *this;
    }
};

int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        CMyistream_iterator<int> inputInt(cin);
        int n1, n2, n3;
        n1 = *inputInt; // 读入 n1
        int tmp = *inputInt;
        cout << tmp << endl;
        inputInt++;
        n2 = *inputInt; // 读入 n2
        inputInt++;
        n3 = *inputInt; // 读入 n3
        cout << n1 << " " << n2 << " " << n3 << " ";
        CMyistream_iterator<string> inputStr(cin);
        string s1, s2;
        s1 = *inputStr;
        inputStr++;
        s2 = *inputStr;
        cout << s1 << " " << s2 << endl;
    }
    return 0;
}
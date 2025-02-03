#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
using namespace std;
const int MAX = 110;
class CHugeInt
{
private:
    char *str;

public:
    CHugeInt(const char *s)
    {
        if (s)
        {
            int length = strlen(s);
            str = new char[210];
            for (int i = 0; i < length; ++i)
                str[i] = s[length - 1 - i];
            str[length] = '\0';
        }
        else
            str = NULL;
    }
    CHugeInt(int n)
    {
        int length = 0;
        str = new char[210];
        while (n)
        {
            str[length++] = n % 10 + '0';
            n = n / 10;
        }
        str[length] = '\0';
    }
    CHugeInt operator+(const CHugeInt &hugeint)
    {
        CHugeInt result("");
        int length1 = strlen(str), length2 = strlen(hugeint.str);
        int length = std::max(length1, length2);
        int carry = 0;
        for (int i = 0; i < length; ++i)
        {
            int temp1 = i < length1 ? str[i] - '0' : 0;
            int temp2 = i < length2 ? hugeint.str[i] - '0' : 0;
            result.str[i] = (temp1 + temp2 + carry) % 10 + '0';
            carry = (temp1 + temp2 + carry) / 10;
        }
        if (carry)
            result.str[length++] = carry + '0';
        result.str[length] = '\0';
        return result;
    }
    friend CHugeInt operator+(int n, const CHugeInt &hugeint)
    {
        return CHugeInt(n) + hugeint;
    }
    CHugeInt operator+(int n)
    {
        return CHugeInt(n) + *this;
    }
    CHugeInt &operator+=(int n)
    {
        *this = *this + n;
        return *this;
    }
    CHugeInt &operator++()
    {
        *this = *this + 1;
        return *this;
    }
    CHugeInt operator++(int)
    {
        CHugeInt result(*this);
        *this = *this + 1;
        return result;
    }
    friend ostream &operator<<(ostream &os, const CHugeInt &hugeint)
    {
        int length = strlen(hugeint.str);
        for (int i = length - 1; i >= 0; --i)
            os << hugeint.str[i];
        return os;
    }
};
int main()
{
    char s[210];
    int n;

    while (cin >> s >> n)
    {
        CHugeInt a(s);
        CHugeInt b(n);

        cout << a + b << endl;
        cout << n + a << endl;
        cout << a + n << endl;
        b += n;
        cout << ++b << endl;
        cout << b++ << endl;
        cout << b << endl;
    }
    return 0;
}
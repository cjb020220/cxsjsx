#include <cstdlib>
#include <iostream>
using namespace std;
int strlen(const char *s)
{
    int i = 0;
    for (; s[i]; ++i)
        ;
    return i;
}
void strcpy(char *d, const char *s)
{
    int i = 0;
    for (i = 0; s[i]; ++i)
        d[i] = s[i];
    d[i] = 0;
}
int strcmp(const char *s1, const char *s2)
{
    for (int i = 0; s1[i] && s2[i]; ++i)
    {
        if (s1[i] < s2[i])
            return -1;
        else if (s1[i] > s2[i])
            return 1;
    }
    return 0;
}
void strcat(char *d, const char *s)
{
    int len = strlen(d);
    strcpy(d + len, s);
}
class MyString
{
private:
    char *str;

public:
    MyString()
    {
        str = NULL;
    }
    MyString(const char *s)
    {
        if (s)
        {
            str = new char[strlen(s) + 1];
            strcpy(str, s);
        }
        else
            str = NULL;
    }
    MyString(const MyString &mystr)
    {
        if (mystr.str)
        {
            str = new char[strlen(mystr.str) + 1];
            strcpy(str, mystr.str);
        }
        else
            str = NULL;
    }
    ~MyString()
    {
        if (str)
            delete[] str;
    }
    MyString &operator=(const char *s)
    {
        if (str)
            delete[] str;
        if (s)
        {
            str = new char[strlen(s) + 1];
            strcpy(str, s);
        }
        else
            str = NULL;
        return *this;
    }
    MyString &operator=(const MyString &mystr)
    {
        if (this == &mystr)
            return *this;
        if (str)
            delete[] str;
        if (mystr.str)
        {
            str = new char[strlen(mystr.str) + 1];
            strcpy(str, mystr.str);
        }
        else
            str = NULL;
        return *this;
    }
    MyString &operator+=(const char *s)
    {
        char *temp = new char[strlen(str) + 1];
        strcpy(temp, str);
        delete[] str;
        str = new char[strlen(temp) + strlen(s) + 1];
        strcpy(str, temp);
        strcat(str, s);
        return *this;
    }
    MyString &operator+=(const MyString &mystr)
    {
        char *temp = new char[strlen(str) + 1];
        strcpy(temp, str);
        delete[] str;
        str = new char[strlen(temp) + strlen(mystr.str) + 1];
        strcpy(str, temp);
        strcat(str, mystr.str);
        return *this;
    }
    MyString operator+(const MyString &mystr)
    {
        MyString result(str);
        result += mystr;
        return result;
    }
    friend MyString operator+(const char *s, const MyString &mystr)
    {
        MyString result(s);
        result += mystr;
        return result;
    }
    bool operator<(const MyString &mystr)
    {
        return strcmp(str, mystr.str) < 0;
    }
    bool operator==(const MyString &mystr)
    {
        return !strcmp(str, mystr.str);
    }
    bool operator>(const MyString &mystr)
    {
        return strcmp(str, mystr.str) > 0;
    }
    char &operator[](int index)
    {
        return str[index];
    }
    char *operator()(int index, int length)
    {
        char *result = new char[length];
        for (int i = index; i < index + length; ++i)
            result[i - index] = str[i];
        result[length] = '\0';
        return result;
    }
    friend ostream &operator<<(ostream &os, const MyString &mystr)
    {
        if (mystr.str)
            os << mystr.str;
        return os;
    }
};

int CompareString(const void *e1, const void *e2)
{
    MyString *s1 = (MyString *)e1;
    MyString *s2 = (MyString *)e2;
    if (*s1 < *s2)
        return -1;
    else if (*s1 == *s2)
        return 0;
    else if (*s1 > *s2)
        return 1;
}
int main()
{
    MyString s1("abcd-"), s2, s3("efgh-"), s4(s1);
    MyString SArray[4] = {"big", "me", "about", "take"};
    cout << "1. " << s1 << s2 << s3 << s4 << endl;
    s4 = s3;
    s3 = s1 + s3;
    cout << "2. " << s1 << endl;
    cout << "3. " << s2 << endl;
    cout << "4. " << s3 << endl;
    cout << "5. " << s4 << endl;
    cout << "6. " << s1[2] << endl;
    s2 = s1;
    s1 = "ijkl-";
    s1[2] = 'A';
    cout << "7. " << s2 << endl;
    cout << "8. " << s1 << endl;
    s1 += "mnop";
    cout << "9. " << s1 << endl;
    s4 = "qrst-" + s2;
    cout << "10. " << s4 << endl;
    s1 = s2 + s4 + " uvw " + "xyz";
    cout << "11. " << s1 << endl;
    qsort(SArray, 4, sizeof(MyString), CompareString);
    for (int i = 0; i < 4; i++)
        cout << SArray[i] << endl;
    // s1的从下标0开始长度为4的子串
    cout << s1(0, 4) << endl;
    // s1的从下标5开始长度为10的子串
    cout << s1(5, 10) << endl;
    return 0;
}
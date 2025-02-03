#include <iostream>
#include <cstring>
using namespace std;

class Array2
{
private:
    int row, column;
    int **num;

public:
    Array2()
    {
        num = NULL;
    }
    Array2(int row_, int column_) : row(row_), column(column_)
    {
        num = new int *[row];
        for (int i = 0; i < row; ++i)
            num[i] = new int[column];
    }
    ~Array2()
    {
        if (num)
        {
            for (int i = 0; i < row; ++i)
                delete[] num[i];
            delete[] num;
        }
    }
    int *operator[](int i)
    {
        return num[i];
    }
    int operator()(int i, int j)
    {
        return num[i][j];
    }
    Array2 &operator=(const Array2 &arr)
    {
        if (this == &arr)
            return *this;
        if (num)
        {
            for (int i = 0; i < row; ++i)
                delete[] num[i];
            delete[] num;
        }
        row = arr.row;
        column = arr.column;
        num = new int *[row];
        for (int i = 0; i < row; ++i)
            num[i] = new int[column];
        for (int i = 0; i < row; ++i)
            memcpy(num[i], arr.num[i], sizeof(int) * column);
        return *this;
    }
};

int main()
{
    Array2 a(3, 4);
    int i, j;
    for (i = 0; i < 3; ++i)
        for (j = 0; j < 4; j++)
            a[i][j] = i * 4 + j;
    for (i = 0; i < 3; ++i)
    {
        for (j = 0; j < 4; j++)
        {
            cout << a(i, j) << ",";
        }
        cout << endl;
    }
    cout << "next" << endl;
    Array2 b;
    b = a;
    for (i = 0; i < 3; ++i)
    {
        for (j = 0; j < 4; j++)
        {
            cout << b[i][j] << ",";
        }
        cout << endl;
    }
    return 0;
}
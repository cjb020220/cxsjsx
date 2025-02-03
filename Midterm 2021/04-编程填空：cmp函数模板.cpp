#include <vector>
#include <set>
#include <list>
#include <iostream>
using namespace std;

template <class T>
void cmp(T a, T b)
{
    if (a < b)
        cout << "a" << endl;
    else
        cout << "b" << endl;
}
template <class T>
void cmp(T *a, T b)
{
    cout << "c" << endl;
}
int main()
{
    int A[100];
    A[0] = 1;
    A[1] = 2;
    cmp(A[0], 2);
    printf("finish 1\n");
    cmp(1, A[1]);
    printf("finish 2\n");
    cmp(A, A[1]);
    printf("finish 3\n");
}
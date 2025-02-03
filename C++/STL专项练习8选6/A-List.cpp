#include <iostream>
#include <list>
#define N 10000
#define LENGTH 10

std::list<int> lst[N];
int n;

int main()
{
    char op[LENGTH];
    int x, y;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i)
    {
        scanf("%s", op);
        if (op[0] == 'n')
            scanf("%d", &x);
        else if (op[0] == 'a')
        {
            scanf("%d%d", &x, &y);
            lst[x].push_back(y);
        }
        else if (op[0] == 'm')
        {
            scanf("%d%d", &x, &y);
            if (x != y)
                lst[x].merge(lst[y]);
        }
        else if (op[0] == 'u')
        {
            scanf("%d", &x);
            lst[x].sort();
            lst[x].unique();
        }
        else
        {
            scanf("%d", &x);
            int length = lst[x].size();
            if (length)
            {
                lst[x].sort();
                for (auto iter : lst[x])
                {
                    printf("%d", iter);
                    --length;
                    if (length)
                        putchar(' ');
                }
            }
            putchar('\n');
        }
    }
}
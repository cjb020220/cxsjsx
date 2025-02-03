#include <iostream>
#include <string.h>
#include <vector>
#include <stack>
#include <queue>
#include <math.h>
#define LENGTH 20

std::stack<double> s;
std::priority_queue<double, std::vector<double>, std::greater<double>> memory;
int n;

void solve()
{
    char str[LENGTH];
    while (scanf("%s", str) != EOF)
    {
        if (strlen(str) == 1 && (str[0] == '+' || str[0] == '-' || str[0] == '*' || str[0] == '/' || str[0] == '^'))
        {
            double y = s.top();
            s.pop();
            double x = s.top();
            s.pop();
            switch (str[0])
            {
            case '+':
                s.push(x + y);
                break;
            case '-':
                s.push(x - y);
                break;
            case '*':
                s.push(x * y);
                break;
            case '/':
                s.push(x / y);
                break;
            default:
                s.push(pow(x, y));
                break;
            }
        }
        else if (str[0] == '=')
        {
            double result = s.top();
            s.pop();
            printf("%e\n", result);
            memory.pop();
            memory.push(result);
        }
        else
            s.push(atof(str));
    }
}

int main()
{
    double value;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i)
    {
        scanf("%lf", &value);
        memory.push(value);
    }
    solve();
    putchar('\n');
    int count = 0;
    while (!memory.empty())
    {
        printf("%e", memory.top());
        memory.pop();
        ++count;
        if (!memory.empty() && count % 10 != 0)
            putchar(' ');
        else
            putchar('\n');
    }
}
#include <iostream>
#include <string>

void dfs(int left, int right, std::string str)
{
    if (!left)
    {
        if (!right)
            std::cout << str << '\n';
        else
            dfs(left, right - 1, str + ')');
        return;
    }
    if (left == right)
        dfs(left - 1, right, str + '(');
    else
    {
        dfs(left - 1, right, str + '(');
        dfs(left, right - 1, str + ')');
    }
}

int main()
{
    int n;
    scanf("%d", &n);
    dfs(n, n, "");
}
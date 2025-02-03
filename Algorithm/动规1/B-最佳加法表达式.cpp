#include <iostream>
#include <string>
#define N 50

std::string dp[N][N];
std::string str;
int n, m;

int mycompare(std::string s, std::string t)
{
    if (t == "inf")
        return -1;
    int length1 = s.size(), length2 = t.size();
    if (length1 < length2)
        return -1;
    else if (length1 > length2)
        return 1;
    else
    {
        for (int i = 0; i < length1; ++i)
        {
            if (s[i] < t[i])
                return -1;
            else if (s[i] > t[i])
                return 1;
        }
        return 0;
    }
}

void reverse(std::string &s)
{
    int left = 0, right = s.size() - 1;
    while (left < right)
        std::swap(s[left++], s[right--]);
}

std::string add(std::string s, std::string t)
{
    reverse(s);
    reverse(t);
    int length1 = s.size(), length2 = t.size();
    int length = std::max(length1, length2);
    int carry = 0;
    std::string result = "";
    for (int i = 0; i < length; ++i)
    {
        int temp1 = i < length1 ? s[i] - '0' : 0;
        int temp2 = i < length2 ? t[i] - '0' : 0;
        result += (temp1 + temp2 + carry) % 10 + '0';
        carry = (temp1 + temp2 + carry) / 10;
    }
    if (carry)
        result += carry + '0';
    reverse(result);
    return result;
}

void solve()
{
    for (int i = 0; i <= m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (j < i)
                continue;
            if (!i)
                dp[i][j] = str.substr(0, j + 1);
            else
            {
                dp[i][j] = "inf";
                for (int k = i - 1; k < j; ++k)
                {
                    std::string temp = add(dp[i - 1][k], str.substr(k + 1, j - k));
                    if (mycompare(temp, dp[i][j]) == -1)
                        dp[i][j] = temp;
                }
            }
        }
    }
    std::cout << dp[m][n - 1] << '\n';
}

int main()
{
    while (scanf("%d", &m) != EOF)
    {
        std::cin >> str;
        n = str.size();
        for (int i = 0; i <= m; ++i)
            for (int j = 0; j < n; ++j)
                dp[i][j] = "";
        solve();
    }
}
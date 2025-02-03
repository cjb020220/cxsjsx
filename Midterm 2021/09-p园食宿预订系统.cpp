#include <iostream>
#include <string>
#include <map>
#define M 6000 + 5

std::map<std::string, int> mymap;
int price[M], total[M];
int result = 0;
int m, n;

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::string name;
    std::cin >> n >> m;
    for (int i = 0; i < m; ++i)
    {
        std::cin >> name >> price[i] >> total[i];
        mymap[name] = i;
    }
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            std::cin >> name;
            int index = mymap[name];
            if (total[index] > 0)
            {
                --total[index];
                result += price[index];
            }
        }
    }
    std::cout << result;
}
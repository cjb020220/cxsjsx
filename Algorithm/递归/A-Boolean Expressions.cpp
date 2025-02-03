#include <iostream>

bool factor();
int cases = 1;
char c;

bool expression()
{
    bool result = factor();
    while (1)
    {
        c = std::cin.peek();
        if (c == '&')
        {
            getchar();
            result = result & factor();
        }
        else if (c == '|')
        {
            getchar();
            result = result | factor();
        }
        else if (c == ' ')
            getchar();
        else
            break;
    }
    return result;
}

bool factor()
{
    bool result;
    while (c = getchar())
    {
        if (c != ' ')
            break;
    }
    if (c == '(')
    {
        result = expression();
        while (c = getchar())
        {
            if (c == ')')
                break;
        }
    }
    else if (c == '!')
        result = !factor();
    else if (c == 'V')
        result = true;
    else if (c == 'F')
        result = false;
    return result;
}

int main()
{
    while (1)
    {
        bool end = false;
        printf("Expression %d: ", cases);
        if (expression())
            printf("V\n");
        else
            printf("F\n");
        while (c = getchar())
        {
            if (c == '\n')
            {
                ++cases;
                break;
            }
            else if (c == EOF)
            {
                end = true;
                break;
            }
        }
        if (end)
            break;
    }
}
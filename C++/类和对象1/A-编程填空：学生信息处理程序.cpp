#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <cstdlib>
using namespace std;

class Student
{
private:
    string name;
    int age, id;
    double grade[4], average = 0;

public:
    void input()
    {
        char c;
        getline(cin, name, ',');
        cin >> age >> c >> id;
        for (int i = 0; i < 4; ++i)
            cin >> c >> grade[i];
    }
    void calculate()
    {
        for (int i = 0; i < 4; ++i)
            average += grade[i];
        average = average / 4;
    }
    void output()
    {
        cout << name << "," << age << "," << id << "," << average << endl;
    }
};

int main()
{
    Student student;     // 定义类的对象
    student.input();     // 输入数据
    student.calculate(); // 计算平均成绩
    student.output();    // 输出数据
}
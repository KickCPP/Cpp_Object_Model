#include <iostream>
#include <typeinfo>

typedef void(*Fun)(void);
typedef unsigned long DWORD;
using namespace std;

class Base
{
public:
    Base(int i) :baseI(i){};
    int getI(){ return baseI; }
    static void countI(){};
    virtual void test1() {cout << "Base::test1()" << endl;}
    virtual void test2() {cout << "Base::test2()" << endl;}
    virtual void print(void){ cout << "Base::print()" << endl; }
private:
    int baseI;
    static int baseS;
};

void testBase(Base &p)
{
    cout << "sizeof(int): " << sizeof(int)
                            << " sizeof(unsigned long): " << sizeof(unsigned long)
                            << " sizeof(DWORD *): " << sizeof(DWORD *) << endl;
    cout << "对象的内存起始地址：" << &p << endl;
    cout << "type_info信息:" << endl;
    DWORD * obj = (DWORD *)&p;
    DWORD **vtbl = (DWORD **)obj[0];
    cout << "虚函数表地址: " << vtbl << endl;
    // 获得type_info对象的指针，并调用其name成员函数
    cout << "\tvtbl[-1]: " << vtbl[-1] << " -> "
                                       << ((type_info*)vtbl[-1])->name() << endl;
    //验证虚表
    cout << "\tvtbl[0](test1): " << vtbl[0] << " -> ";
    ((Fun)vtbl[0])();
    cout << "\tvtbl[1](test2): " << vtbl[1] << " -> ";
    ((Fun)vtbl[1])();
    cout << "\tvtbl[2](print): " << vtbl[2] << " -> ";
    ((Fun)vtbl[2])();

    //输入static函数的地址
    p.countI();//先调用函数以产生一个实例
    cout << "static函数countI()的地址：" << p.countI << endl;

    //验证nonstatic数据成员
    cout << "推测nonstatic数据成员baseI的地址：" << &obj[1] << endl;
    cout << "根据推测出的地址，输出该地址的值：" << *(int *)(&obj[1]) << endl;
    cout << "Base::getI():" << p.getI() << endl;
}

int main()
{
    Base b(1000);
    testBase(b);
}

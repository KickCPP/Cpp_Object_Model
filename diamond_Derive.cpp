#include <iostream>
#include <typeinfo>

typedef void(*Fun)(void);
typedef unsigned long DWORD;
using namespace std;

class B {
public:
    int ib;
    B(int i=1): ib(i) {}
    virtual void f(){cout << "B::f()" << endl;}
    virtual void Bf(){cout << "B::Bf()" << endl;}
};

class B1: public B {
public:
    int ib1;
    B1(int i=100): ib1(i) {}
    virtual void f(){cout << "B1::f()" << endl;}
    virtual void f1(){cout << "B1::f1()" << endl;}
    virtual void Bf1(){cout << "B1::Bf1()" << endl;}
};

class B2: public B {
public:
    int ib2;
    B2(int i=1000): ib2(i) {}
    virtual void f(){cout << "B2::f()" << endl;}
    virtual void f2(){cout << "B2::f2()" << endl;}
    virtual void Bf2(){cout << "B2::Bf2()" << endl;}
};

class D: public B1, public B2 {
public:
    int id;
    D(int i=10000): id(i) {}
    virtual void f(){cout << "D::f()" << endl;}
    virtual void f1(){cout << "D::f1()" << endl;}
    virtual void f2(){cout << "D::f2()" << endl;}
    virtual void Df(){cout << "D::Df()" << endl;}
};

void testDiamondDerive(D &p)
{
    cout << "sizeof(int): " << sizeof(int)
                            << " sizeof(unsigned long): " << sizeof(unsigned long)
                            << " sizeof(DWORD *): " << sizeof(DWORD *) << endl;
    DWORD * obj = (DWORD *)&p;
    DWORD **vtbl = (DWORD **)obj[0];
    // 获得type_info对象的指针，并调用其name成员函数
    cout << "[0]B::vptr\t Addr: " << vtbl << endl;
    cout << "\tvtbl[-1](type_info pointer):" << vtbl[-1] << " -> "
                                       << ((type_info*)vtbl[-1])->name() << endl;
    //验证虚表
    cout <<"\tvtbl[0](D::f): " << vtbl[0] << " -> ";
    ((Fun)vtbl[0])();
    cout <<"\tvtbl[1](B::Bf): " << vtbl[1] << " -> ";
    ((Fun)vtbl[1])();
    cout <<"\tvtbl[2](D::f1): " << vtbl[2] << " -> ";
    ((Fun)vtbl[2])();
    cout <<"\tvtbl[3](B1::Bf1): " << vtbl[3] << " -> ";
    ((Fun)vtbl[3])();
    cout <<"\tvtbl[4](D::f2): " << vtbl[4] << " -> ";
    ((Fun)vtbl[4])();
    cout <<"\tvtbl[5](D::Df): " << vtbl[5] << " -> ";
    ((Fun)vtbl[5])();

    //验证nonstatic数据成员
    cout << "[1]B::ib：" << &obj[1] << " -> " <<  *(int *)(&obj[1]) << endl;
    cout << "[2]B1::ib1：" << (int *)(&obj[1]) + 1<< " -> " <<  *((int *)(&obj[1]) + 1) << endl;
    
    DWORD * obj2 = (DWORD *)(obj + 2); // 内存对齐
    DWORD **vtbl2 = (DWORD **)obj2[0];  
    // 获得type_info对象的指针，并调用其name成员函数
    cout << "[3]B1::vptr\t Addr: " << vtbl2 << endl;
    cout << "\tvtbl2[-1](type_info pointer):" << vtbl2[-1] << " -> "
                                       << ((type_info*)vtbl2[-1])->name() << endl;
    //验证虚表
    cout <<"\tvtbl2[0](D::f): " << vtbl2[0] << " -> ";
    ((Fun)vtbl2[0])();
    cout <<"\tvtbl2[1](B::Bf): " << vtbl2[1] << " -> ";
    ((Fun)vtbl2[1])();
    cout <<"\tvtbl2[2](D::f2): " << vtbl2[2] << " -> ";
    ((Fun)vtbl2[2])();
    cout <<"\tvtbl2[3](B2::Bf2): " << vtbl2[3] << " -> ";
    ((Fun)vtbl2[3])();
    
    //验证nonstatic数据成员
    cout << "[4]B::ib：" << &obj[3] << " -> " <<  *(int *)(&obj[3]) << endl;
    cout << "[5]B2::ib2：" << (int *)(&obj[3]) + 1<< " -> " <<  *((int *)(&obj[3]) + 1) << endl;
    cout << "[6]D::id：" << &obj[4] << " -> " <<  *(int *)(&obj[4]) << endl;
}

int main()
{
    D d;
    testDiamondDerive(d);
}

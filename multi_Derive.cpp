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
    virtual ~Base() {}
    virtual void print(void){ cout << "Base::print()" << endl; }
private:
    int baseI;
    static int baseS;
};

class Base_2
{
public:
    Base_2(int i) :base2I(i){};
    int getI(){ return base2I; }
    static void countI(){};
    virtual ~Base_2() {}
    virtual void print(void){ cout << "Base_2::print()" << endl; }
private:
    int base2I;
    static int base2S;
};


class MultiDerive : public Base, public Base_2
{
public:
    MultiDerive(int d): Base(1000), Base_2(2000), Derive2I(d) {}
    virtual void print() { cout << "MultiDerive::print()" << endl;}
    virtual void Derive_print() {cout << "MultiDerive::Derive_print()" << endl;}
    virtual ~MultiDerive() {}
private:
    int Derive2I;
};

void testDerive(Base &p)
{
    cout << "sizeof(int): " << sizeof(int)
                            << " sizeof(unsigned long): " << sizeof(unsigned long)
                            << " sizeof(DWORD *): " << sizeof(DWORD *) << endl;
    DWORD * obj = (DWORD *)&p;
    DWORD **vtbl = (DWORD **)obj[0];
    // 获得type_info对象的指针，并调用其name成员函数
    cout << "[0]Base::vptr\t Addr: " << vtbl << endl;
    cout << "\tvtable[-1](type_info pointer):" << vtbl[-1] << " -> "
                                       << ((type_info*)vtbl[-1])->name() << endl;
    //验证虚表
    cout << "\tvtbl[0](destructor): " << vtbl[0] << " -> MultiDerive::~MultiDerive" << endl;
    cout << "\tvtbl[1](destructor): " << vtbl[1] << " -> MultiDerive::~MultiDerive" << endl;
    cout << "\tvtbl[2](print): " << vtbl[2] << " -> ";
    ((Fun)vtbl[2])();
    cout << "\tvtbl[3](Derive_print): " << vtbl[3] << " -> ";
    ((Fun)vtbl[3])();

    //验证nonstatic数据成员
    cout << "[1]Base::baseI：" << &obj[1] << " -> " <<  *(int *)(&obj[1]) << endl;
    
    DWORD * obj2 = (DWORD *)(obj + 2); // 内存对齐
    DWORD **vtbl2 = (DWORD **)obj2[0];  

    cout << "[2]Base_2::vptr\tAddr: " << vtbl2 << endl;
    
    //验证虚表
    cout << "\tvtbl[0](destructor): " << vtbl2[0] << " -> MultiDerive::~MultiDerive" << endl;
    cout << "\tvtbl[1](destructor): " << vtbl2[1] << " -> MultiDerive::~MultiDerive" << endl;
    cout << "\tvtbl[2](print): " << vtbl2[2] << " -> ";
    ((Fun)vtbl2[2])();
    
    //验证nonstatic数据成员
    cout << "[3]Base_2::base2I：" << &obj2[1] << " -> " << *(int *)(&obj2[1]) << endl;
    cout << "[4]MultiDerive::Derive2I：" << (int *)&obj2[1] + 1 << " -> " << *(int *)((int *)(&obj2[1]) + 1)  << endl;
    
    // 验证static函数成员
    p.countI();//先调用函数以产生一个实例
    cout << "Addr of countI()：" << p.countI << endl;
}

int main()
{
    MultiDerive d(100);
    testDerive(d);
    Base b1(1000);
    b1.countI();
    cout << "Addr of countI()：" << b1.countI << endl;
    Base_2 b2(2000);
    b2.countI();
    cout << "Addr of countI()：" << b2.countI << endl;
}

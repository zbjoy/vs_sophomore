#include <iostream>

using namespace std;

class Animal
{
public:
    Animal()
    {
        mA = 100;
    }

public:

    int mA;
};

class Sheep : virtual public Animal
{

};

class Clem : virtual public Animal
{

};

class SheepClem : public Sheep, public Clem
{

};


void test()
{
    Sheep cm;
    cout << (((Animal*)((char*)(&cm) + (*(((int*)(*((long long*)(&cm)))) + 1))))->mA) << endl;
    //cout << ((Animal*)((char*)&cm + *((int*)*(int*)&cm + 1)))->mA << endl;
}

int main()
{
    test();
    return 0;
}
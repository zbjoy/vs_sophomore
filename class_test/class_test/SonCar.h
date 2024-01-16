#pragma once
#include "Car.h"
class SonCar :
    public Car
{
public:
    void print();
    void setInformation(string tName, int tAge, string tType, string tNumber);

private:
    string number;
};


#include "Car.h"

void Car::print()
{
	cout << name << "  " << age << "  " << type;
}

void Car::setInformation(string tName, int tAge, string tType)
{
	name = tName;
	age = tAge;
	type = tType;
}
